#include "Window.h"

#include "Timer.h"

#include "glad/glad.h"

#include "../imgui/imgui_impl_opengl3.h"
#include "../imgui/imgui_impl_sdl.h"
#include "../imgui/imgui_stdlib.h"

#include <chrono>
#include <string>

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

	SDL_GL_SetSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
		return;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glViewport(0, 0, mWidth, mHeight);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	mIo = ImGui::GetIO();

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(mWindow, mGLContext);
	ImGui_ImplOpenGL3_Init("#version 460 core");

	mInitSuccess = true;
}

Window::~Window() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(mGLContext);

	if (mWindow != nullptr)
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
			h = (bounds.y - 30.0f) * 0.5;
			ImGui::BeginChild("Debug", ImVec2(w, h), true);

			drawDebugPanel();

			ImGui::EndChild();

			h = ImGui::GetContentRegionAvail().y - 30.0f;
			ImGui::BeginChild("Config", ImVec2(w, h), true);

			drawIOPanel();

			ImGui::EndChild();

			ImGui::EndGroup();
			ImGui::SameLine();
			ImGui::BeginGroup();

			bounds = ImGui::GetContentRegionAvail();
			w = bounds.x;
			h = bounds.y - 30.0f;
			ImGui::BeginChild("Simulation", ImVec2(w, h), true);

			drawSimPanel();

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
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym) {
				case SDLK_ESCAPE:
					mRunning = false;
					break;
			}
			break;
	}
}

void Window::drawDebugPanel() {
	ImGui::TextColored(
		ImVec4(1.0f, 1.0f, 0.0f, 1.0f),
		"[FPS: %.2f | %.2fms]",
		Timer::getTimer().getFPS(), Timer::getTimer().getMS()
	);
}

void Window::drawIOPanel() {
}

void Window::drawSimPanel() {
}
