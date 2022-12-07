#include "LLCA3DRenderer.h"

const GLchar* LLCA3Dvert =
#include "../Shaders/texQuad.vert"
;
const GLchar* LLCA3Dfrag =
#include "../Shaders/LLCA3D.frag"
;

LLCA3DRenderer::LLCA3DRenderer(Window& window, GLuint& texture) :
mSimulator(), Renderer(window, mSimulator, texture), mShader(&LLCA3Dvert, &LLCA3Dfrag) {
}

LLCA3DRenderer::~LLCA3DRenderer() {
}

void LLCA3DRenderer::drawParameters() {
}

void LLCA3DRenderer::drawImageConfig() {
}

void LLCA3DRenderer::focusAction() {
}

void LLCA3DRenderer::updateSim() {
}

void LLCA3DRenderer::drawSim() {
}

void LLCA3DRenderer::imageResized() {
}
