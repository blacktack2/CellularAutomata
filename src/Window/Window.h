#pragma once
#include "Renderer.h"

#include "imgui/imgui.h"

#ifdef WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

class Window {
public:
	Window(const char* title, int width, int height);
	~Window();

	void mainloop();

	inline bool hasInitialized() {
		return mInitSuccess;
	}
private:
	void handleEvent(SDL_Event& e);

	void drawDebugPanel(float dt);
	void drawIOPanel(float dt);
	void drawSimPanel(float dt);

	SDL_Window* mWindow = nullptr;
	SDL_GLContext mGLContext{};
	ImGuiIO mIo;

	Renderer* mRenderer = nullptr;

	GLuint mSimTexture;

	ImVec2 mSimArea = ImVec2(100, 100);

	int mWidth;
	int mHeight;

	bool mRunning = false;
	bool mInitSuccess = false;

	bool mRunSimulation = false;
	float mIterationDelay = 0;
};