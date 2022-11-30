#include "Window.h"

#include "LLCA2DSerializer.h"
#include "LLCA2DRenderer.h"
#include "Timer.h"
#include "../Simulation/GLUtils.h"

#include "glad/glad.h"

#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_stdlib.h"

#include <chrono>
#include <string>

bool compareStrings(const std::string& sa, const std::string& sb) {
	const char* a = sa.data();
	const char* b = sb.data();
	while (true) {
		if (*a == '\0')
			return true;
		if (*b == '\0')
			return false;
		if (*a < *b)
			return true;
		else if (*a > *b)
			return false;
		a++;
		b++;
	}
}

Window::Window(const char* title, int width, int height) :
mWidth(width), mHeight(height) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return;

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

	auto windowFlags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

	mWindow = SDL_CreateWindow("Cellular Automata", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mWidth, mHeight, windowFlags);
	if (mWindow == nullptr)
		return;

	SDL_SetWindowMinimumSize(mWindow, 800, 600);
	mGLContext = SDL_GL_CreateContext(mWindow);

    mAllowVsync = !SDL_GL_SetSwapInterval(1);
    if (mAllowVsync)
	    mAllowAdaptiveVsync = !SDL_GL_SetSwapInterval(-1);
	SDL_GL_SetSwapInterval((mVsync = mAllowVsync) ? ((mAdaptiveVsync = mAllowAdaptiveVsync) ? -1 : 1) : 0);

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
		return;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glViewport(0, 0, mWidth, mHeight);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(mWindow, mGLContext);
	ImGui_ImplOpenGL3_Init("#version 460 core");

	glGenTextures(1, &mSimTexture);
	glBindTexture(GL_TEXTURE_2D, mSimTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mSimArea.x, mSimArea.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);

#ifdef _DEBUG
	glCheckError();
#endif

	mRenderer = new LLCA2DRenderer(*this, mSimTexture);
	mSerializer = new LLCA2DSerializer(*(LLCA2DRenderer*)mRenderer, *(LLCA2DSimulator*)&mRenderer->getSimulator());
	mSerializer->find(mConfigFiles);
	std::sort(mConfigFiles.begin(), mConfigFiles.end(), compareStrings);

	mInitSuccess = true;
}

Window::~Window() {
    delete mRenderer;

	glDeleteTextures(1, &mSimTexture);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(mGLContext);

	if (mWindow)
		SDL_DestroyWindow(mWindow);

	SDL_Quit();
}

void Window::mainloop() {
	SDL_Event e;

	mRunning = true;
	while (mRunning) {
		float dt = Timer::getTimer().getFrameDelta().count();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		while (SDL_PollEvent(&e))
			handleEvent(e);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(mWindow);
		ImGui::NewFrame();
		{
			ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
			ImGui::SetNextWindowSize(ImVec2((float)mWidth, (float)mHeight), ImGuiCond_Always);
			ImGui::Begin("##Root", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
			ImVec2 bounds;
			float w, h;

			ImGui::BeginGroup();

			bounds = ImGui::GetContentRegionAvail();
			w = bounds.x * (2.0f / 7.0f);
			h = (bounds.y - 30.0f) * 0.2;
			ImGui::BeginChild("Debug", ImVec2(w, h), true);

			drawDebugPanel(dt);

			ImGui::EndChild();

			h = (ImGui::GetContentRegionAvail().y - 30.0f) / 3.0f;
			ImGui::BeginChild("IO", ImVec2(w, h), true);

			drawIOPanel(dt);

			ImGui::EndChild();

			h = ImGui::GetContentRegionAvail().y - 30.0f;
			ImGui::BeginChild("Parameter", ImVec2(w, h), true);

			drawParameterPanel(dt);

			ImGui::EndChild();

			ImGui::EndGroup();
			ImGui::SameLine();
			ImGui::BeginGroup();

			bounds = ImGui::GetContentRegionAvail();
			w = bounds.x;
			h = bounds.y - 30.0f;
			ImGui::BeginChild("Simulation", ImVec2(w, h), true);

			drawSimPanel(dt);

			ImGui::EndChild();

			ImGui::EndGroup();
			ImGui::BeginGroup();

			bounds = ImGui::GetContentRegionAvail();
			w = bounds.x;
			h = bounds.y;
			ImGui::BeginChild("Message", ImVec2(w, h), true);

			ImGui::EndChild();

			ImGui::EndGroup();

			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		SDL_GL_SwapWindow(mWindow);
	}
}

void Window::handleEvent(SDL_Event& e) {
	ImGui_ImplSDL2_ProcessEvent(&e);

	switch (e.type) {
		case SDL_QUIT:
			mRunning = false;
			break;
		case SDL_WINDOWEVENT:
			switch (e.window.event) {
				case SDL_WINDOWEVENT_SIZE_CHANGED:
					mWidth = e.window.data1;
					mHeight = e.window.data2;
					glViewport(0, 0, mWidth, mHeight);
					break;
			}
			break;
	}
	if (ImGui::IsKeyPressed(ImGuiKey_Escape))
		mRunning = false;
}

void Window::drawDebugPanel(float dt) {
	ImGui::TextColored(
		ImVec4(1.0f, 1.0f, 0.0f, 1.0f),
		"[FPS: %.2f | %.2fms]",
		Timer::getTimer().getFPS(), Timer::getTimer().getMS()
	);

	if (mAllowVsync) {
	    if (ImGui::Checkbox("Vsync", &mVsync))
            SDL_GL_SetSwapInterval(mVsync ? (mAdaptiveVsync ? -1 : 1) : 0);
	    if (mAllowAdaptiveVsync) {
	        ImGui::BeginDisabled(!mVsync);
	        if (ImGui::Checkbox("Adaptive Vsync", &mAdaptiveVsync))
                SDL_GL_SetSwapInterval(mVsync ? (mAdaptiveVsync ? -1 : 1) : 0);
	        ImGui::EndDisabled();
	    }
	}
}

void Window::drawIOPanel(float dt) {
	ImVec2 bounds = ImGui::GetContentRegionAvail();
	ImGui::PushItemWidth(-FLT_MIN);

	mRunSimulation ^= ImGui::Button(mRunSimulation ? "Stop Simulation" : "Start Simulation", ImVec2(-FLT_MIN, 0));

	if (ImGui::Button("Reset", ImVec2(-FLT_MIN, 0)))
		mRenderer->getSimulator().reset();

	ImGui::Text("Iteration Delay");
	ImGui::DragFloat("##IterationDelay", &mIterationDelay, 10.0f, 0.0f, 2000.0f, "%.0f ms");

	ImGui::Separator();

	if (ImGui::Button("Load"))
		mSerializer->read(mConfigFiles[mSelectedLoadFile]);
	ImGui::SameLine(0, 0);
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Delete ").x);
	ImGui::Combo("##SelectFile", &mSelectedLoadFile,
		[](void* data, int n, const char** out) {
			std::string& file = ((std::string*)data)[n];
			*out = file.c_str();
			return true;
		},
		mConfigFiles.data(), mConfigFiles.size());
	ImGui::SameLine(0, 0);
	if (ImGui::Button("Delete", ImVec2(-FLT_MIN, 0))) {
		mSerializer->remove(mConfigFiles[mSelectedLoadFile]);
		mConfigFiles.erase(mConfigFiles.begin() + mSelectedLoadFile);
		mSelectedLoadFile = std::clamp(mSelectedLoadFile - 1, 0, (int)mConfigFiles.size());
	}

	ImGui::Separator();

	if (ImGui::Button("Save")) {
		mSerializer->write(mSelectedSaveFile);
		mConfigFiles.emplace_back(mSelectedSaveFile);
		std::sort(mConfigFiles.begin(), mConfigFiles.end(), compareStrings);
	}
	ImGui::SameLine(0, 0);
	ImGui::InputText("##SaveLocation", &mSelectedSaveFile);

	ImGui::PopItemWidth();
}

void Window::drawParameterPanel(float dt) {
	ImGui::PushItemWidth(-FLT_MIN);

	mRenderer->drawParameters();

	ImGui::PopItemWidth();
}

void Window::drawSimPanel(float dt) {
	static float delta = 0;

	if (ImGui::IsKeyPressed(ImGuiKey_Space))
		mRunSimulation = !mRunSimulation;
	if (mRunSimulation) {
		delta += dt;
		if (delta > mIterationDelay) {
			delta -= mIterationDelay;
			mRenderer->update();
		}
	}

	ImVec2 simArea = ImGui::GetContentRegionAvail();
	if (simArea.x != mSimArea.x || simArea.y != mSimArea.y) {
		mSimArea = simArea;
		glBindTexture(GL_TEXTURE_2D, mSimTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mSimArea.x, mSimArea.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
#ifdef _DEBUG
		glCheckError();
#endif
		ImVec2 screenPos = ImGui::GetCursorScreenPos();
		mRenderer->resizeImage(glm::vec4(screenPos.x, screenPos.y, mSimArea.x, mSimArea.y));
	}

	glViewport(0, 0, mSimArea.x, mSimArea.y);
	mRenderer->draw();
	glViewport(0, 0, mWidth, mHeight);

	ImGui::Image((ImTextureID)(uintptr_t)mSimTexture, mSimArea);
	if (ImGui::IsItemHovered())
		mRenderer->focusAction();
}
