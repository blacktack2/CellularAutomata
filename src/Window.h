#include "../imgui/imgui.h"

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

	inline bool hasInitialized() { return mInitSuccess; }
private:
	void handleEvent(SDL_Event& e);

	SDL_Window* mWindow = nullptr;
	SDL_GLContext mGLContext{};
	ImGuiIO mIo;

	int mWidth;
	int mHeight;

	bool mRunning = false;
	bool mInitSuccess = false;
};