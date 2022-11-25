#include "Renderer.h"

#include "Window.h"

Renderer::Renderer(Window& window, Simulator& simulator, GLuint& texture) :
mWindow(window), mSimulator(simulator), mTexture(texture) {
	glGenFramebuffers(1, &mFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mTexture, 0);
	GLenum drawBuffers = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, &drawBuffers);

#ifdef _DEBUG
	GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		return;
#endif

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenVertexArrays(1, &mQuadArrayObject);
	glBindVertexArray(mQuadArrayObject);

	glGenBuffers(1, &mQuadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mQuadVBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec2), mQuadVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

Renderer::~Renderer() {
	glDeleteFramebuffers(1, &mFBO);
	glDeleteVertexArrays(1, &mQuadArrayObject);

	delete[] mQuadVertices;
}

void Renderer::update() {
	updateSim();
}

void Renderer::draw() {
	drawSim();
}

void Renderer::drawQuad() {
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(mQuadVBO);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
