#pragma once
#include "../Simulation/Mesh.h"
#include "../Simulation/Simulator.h"

#include <glad/glad.h>

#include <glm/vec2.hpp>

class Window;

class Renderer {
public:
	Renderer(Window& window, Simulator& simulator, GLuint& texture);
	~Renderer();

	void update();
	void draw();
	virtual void drawParameters() = 0;

	inline Simulator& getSimulator() { return mSimulator; }
protected:
	virtual void updateSim() = 0;
	virtual void drawSim() = 0;

	void drawQuad();

	Window& mWindow;
	Simulator& mSimulator;

	bool initSuccess = false;
private:
	GLuint mFBO;
	GLuint& mTexture;

	Mesh* mScreenQuad = Mesh::getScreenQuad();
};
