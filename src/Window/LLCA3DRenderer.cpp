#include "LLCA3DRenderer.h"

#include "IconAtlasHandler.h"

const GLchar* LLCA3Dvert =
#include "../Shaders/LLCA3D.vert"
;
const GLchar* LLCA3Dfrag =
#include "../Shaders/LLCA3D.frag"
;

LLCA3DRenderer::LLCA3DRenderer(Window& window, GLuint& texture) :
mSimulator(), Renderer(window, mSimulator, texture), mShader(&LLCA3Dvert, &LLCA3Dfrag) {
	mShader.bind();

	mViewOffsetUniform = glGetUniformLocation(mShader.getProgram(), "viewOffset");
	mSimBoundsUniform  = glGetUniformLocation(mShader.getProgram(), "simulationBounds");
	mImageBoundsUniform  = glGetUniformLocation(mShader.getProgram(), "imageBounds");
	mInvNumGenerationsUniform = glGetUniformLocation(mShader.getProgram(), "invNumGenerations");

	mLiveColUniform = glGetUniformLocation(mShader.getProgram(), "liveCol");
	mDeadColUniform = glGetUniformLocation(mShader.getProgram(), "deadCol");
	mBorderColUniform = glGetUniformLocation(mShader.getProgram(), "borderCol");
	mGridColUniform = glGetUniformLocation(mShader.getProgram(), "gridCol");

	glUniform3uiv(mSimBoundsUniform, 1, &mSimulator.getBounds()[0]);
	glUniform2fv(mImageBoundsUniform, 1, &mImageBounds[0]);
	glUniform1f(mInvNumGenerationsUniform, 1.0f / mSimulator.getNumGenerations());

	glUniform3fv(mLiveColUniform, 1, &mLiveColour[0]);
	glUniform3fv(mDeadColUniform, 1, &mDeadColour[0]);

	mShader.unbind();
}

LLCA3DRenderer::~LLCA3DRenderer() {
	delete mCube;
}

void LLCA3DRenderer::drawParameters() {
}

void LLCA3DRenderer::drawImageConfig() {
	IconAtlasHandler::AtlasMap* map = IconAtlasHandler::getAtlasMap("Arrows");
	IconAtlasHandler::Icon& icon = map->icons.find("arrow_all [#328]")->second;
	if (ImGui::ImageButton("ResetView", (ImTextureID)map->atlas.id, ImVec2(16, 16), icon.uv1, icon.uv2)) {
	}
}

void LLCA3DRenderer::focusAction() {
	ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
}

void LLCA3DRenderer::updateSim() {
	mSimulator.iterate();
}

void LLCA3DRenderer::drawSim() {
	mShader.bind();
	mCube->drawInstanced(mSimulator.getBounds().x * mSimulator.getBounds().y * mSimulator.getBounds().z);
	mShader.unbind();
}

void LLCA3DRenderer::imageResized() {
}
