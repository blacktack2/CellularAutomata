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
	void resizeImage(glm::vec4 imageBounds);
	virtual void drawParameters() = 0;
	virtual void drawImageConfig() = 0;
	virtual void focusAction() = 0;

	inline Simulator& getSimulator() { return mSimulator; }
protected:
	virtual void updateSim() = 0;
	virtual void drawSim() = 0;
	virtual void imageResized() = 0;

	void drawQuad();

	Window& mWindow;
	Simulator& mSimulator;

	glm::vec4 mImageBounds = glm::vec4(0.0f, 0.0f, 100.0f, 100.0f);

	bool mInitSuccess = false;
private:
	GLuint mFBO;
	GLuint& mTexture;

	Mesh* mScreenQuad = Mesh::getScreenQuad();
};
