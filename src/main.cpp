#include "Window.h"

int main(int argc, char* argv[]) {
	Window w = Window("Cellular Automata", 800, 600);
	if (!w.hasInitialized())
		return -1;
	w.mainloop();
	return 0;
}
