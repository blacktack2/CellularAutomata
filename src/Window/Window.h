#pragma once
#include "FileHandler.h"
#include "Renderer.h"

#include "imgui/imgui.h"

#ifdef WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include <string>
#include <vector>

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
	void drawParameterPanel(float dt);
	void drawSimPanel(float dt);

	SDL_Window* mWindow = nullptr;
	SDL_GLContext mGLContext{};
	ImGuiIO mIo;

	Renderer* mRenderer = nullptr;
	FileHandler* mFileHandler = nullptr;

	GLuint mSimTexture;

	ImVec2 mSimArea = ImVec2(100, 100);

	int mWidth;
	int mHeight;

	bool mRunning = false;
	bool mInitSuccess = false;

	bool mVsync = false;
	bool mAdaptiveVsync = false;
	bool mAllowVsync = false;
	bool mAllowAdaptiveVsync = false;

	bool mRunSimulation = false;
	float mIterationDelay = 0;

	std::string mSelectedSaveFile = "myconfig";
	int mSelectedLoadFile = 0;
	std::vector<char*> mConfigFiles{};
};