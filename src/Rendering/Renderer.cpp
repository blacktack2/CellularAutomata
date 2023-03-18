#include "Renderer.h"

#include "../Window/Window.h"

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

	glBindVertexArray(0);
}

Renderer::~Renderer() {
	glDeleteFramebuffers(1, &mFBO);
}

void Renderer::update() {
	updateSim();
}

void Renderer::draw() {
	drawSim();
}

void Renderer::resizeImage(glm::vec4 imageBounds) {
	mImageBounds = imageBounds;
	imageResized();
}

void Renderer::drawQuad() {
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glClear(GL_COLOR_BUFFER_BIT);

	mScreenQuad->draw();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
