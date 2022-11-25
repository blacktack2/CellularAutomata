#pragma once
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

	GLuint mQuadArrayObject;
	GLuint mQuadVBO;

	glm::vec2* mQuadVertices = new glm::vec2[4]{
		glm::vec2(-1.0f, -1.0f), glm::vec2(1.0f, -1.0f), glm::vec2(-1.0f, 1.0f), glm::vec2(1.0f, 1.0f)
	};
};
