#include "LLCA2DRenderer.h"

#include "IconAtlasHandler.h"
#include "Window.h"

#include <cmath>
#include <string>

const GLchar* LLCA2Dvert =
#include "../Shaders/texQuad.vert"
;
const GLchar* LLCA2Dfrag =
#include "../Shaders/LLCA2D.frag"
;

LLCA2DRenderer::LLCA2DRenderer(Window& window, GLuint& texture) :
mSimulator(), Renderer(window, mSimulator, texture), mShader(&LLCA2Dvert, &LLCA2Dfrag) {
	mShader.bind();
	
	mViewOffsetUniform = glGetUniformLocation(mShader.getProgram(), "viewOffset");
	mSimBoundsUniform  = glGetUniformLocation(mShader.getProgram(), "simulationBounds");
	mImageBoundsUniform  = glGetUniformLocation(mShader.getProgram(), "imageBounds");
	mInvNumGenerationsUniform = glGetUniformLocation(mShader.getProgram(), "invNumGenerations");

	mLiveColUniform = glGetUniformLocation(mShader.getProgram(), "liveCol");
	mDeadColUniform = glGetUniformLocation(mShader.getProgram(), "deadCol");
	mBorderColUniform = glGetUniformLocation(mShader.getProgram(), "borderCol");
	mGridColUniform = glGetUniformLocation(mShader.getProgram(), "gridCol");

	glUniform4fv(mViewOffsetUniform, 1, &mViewOffset[0]);
	glUniform2uiv(mSimBoundsUniform, 1, &mSimulator.getBounds()[0]);
	glUniform2fv(mImageBoundsUniform, 1, &mImageBounds[0]);
	glUniform1f(mInvNumGenerationsUniform, 1.0f / mSimulator.getNumGenerations());

	glUniform3fv(mLiveColUniform, 1, &mLiveColour[0]);
	glUniform3fv(mDeadColUniform, 1, &mDeadColour[0]);
	glUniform4fv(mBorderColUniform, 1, &mBorderColour[0]);
	glUniform4fv(mGridColUniform, 1, &mGridColour[0]);

	mShader.unbind();
}

LLCA2DRenderer::~LLCA2DRenderer() {
}

void LLCA2DRenderer::drawParameters() {
	static const ImVec2 REMAINING_WIDTH = ImVec2(-FLT_MIN, 0);
	ImVec2 bounds = ImGui::GetContentRegionAvail();
	std::string label;

	ImGui::Text("Simulation Bounds");
	glm::uvec2 simBounds = mSimulator.getBounds();
	if (ImGui::DragInt2("##bounds", (int*)&simBounds[0], 10, 2, LLCA2DSimulator::cMaxSize)) {
		setBounds(simBounds);
	}

    ImGui::Text("Init Mode");
    const char* INIT_MODE_NAMES[]{ "Random", "Single", "Horizontal", "Vertical", "Cross" };
    LLCA2DSimulator::InitMode initMode = mSimulator.getInitMode();
    if (ImGui::Combo("##InitMode", (int*)&initMode, INIT_MODE_NAMES, (int)LLCA2DSimulator::InitMode::MAX)) {
        mSimulator.setInitMode(initMode);
    }

	ImGui::Text("Edge Mode");
	const char* EDGE_MODE_NAMES[]{ "Clamp", "Wrap", "Dead", "Live" };
	LLCA2DSimulator::EdgeMode edgeMode = mSimulator.getEdgeMode();
	if (ImGui::Combo("##EdgeMode", (int*)&edgeMode, EDGE_MODE_NAMES, (int)LLCA2DSimulator::EdgeMode::MAX)) {
		mSimulator.setEdgeMode(edgeMode);
	}

	ImGui::Text("Live Colour");
	if (ImGui::ColorEdit3("##LiveColour", (float*)&mLiveColour[0])) {
		setLiveColour(mLiveColour);
	}

	ImGui::Text("Dead Colour");
	if (ImGui::ColorEdit3("##DeadColour", (float*)&mDeadColour[0])) {
		setDeadColour(mDeadColour);
	}

	ImGui::Separator();

	float ruleBoxWidth = std::floor(bounds.x * (1.0f / 9.0f));
	ImVec4 liveColor = ImVec4(mLiveColour.r, mLiveColour.g, mLiveColour.b, 1.0f);
	ImVec4 deadColor = ImVec4(mDeadColour.r, mDeadColour.g, mDeadColour.b, 1.0f);

	ImGui::Text("Birth Rules");
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Whether a dead cell should become alive or stay dead with 'n' neighbours");
	LLCA2DSimulator::ruleset birthRules = mSimulator.getBirthRules();
	LLCA2DSimulator::ruleset newBirthRules = birthRules;
	
	for (unsigned int i = 0; i < 9; i++) {
		bool rule = (birthRules & (1 << i));
		label = std::to_string(i).append("##BirthRule-").append(std::to_string(i));
		if (rule) {
			ImGui::PushStyleColor(ImGuiCol_Button, liveColor);
			ImGui::PushStyleColor(ImGuiCol_Text,   deadColor);
		} else {
			ImGui::PushStyleColor(ImGuiCol_Button, deadColor);
			ImGui::PushStyleColor(ImGuiCol_Text,   liveColor);
		}
		newBirthRules ^= (ImGui::Button(label.c_str(), ImVec2(ruleBoxWidth, 0)) ? 1 : 0) << i;
		ImGui::PopStyleColor(2);
		if (i < 8)
			ImGui::SameLine(0.0f, 0.0f);
	}

	if (birthRules != newBirthRules)
		mSimulator.setBirthRules(newBirthRules);

	ImGui::Text("Death Rules");
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Whether a live cell should stay alive or become dead with 'n' neighbours");
	LLCA2DSimulator::ruleset deathRules = mSimulator.getDeathRules();
	LLCA2DSimulator::ruleset newDeathRules = deathRules;

	for (unsigned int i = 0; i < 9; i++) {
		bool rule = deathRules & (1 << i);
		label = std::to_string(i).append("##DeathRule-").append(std::to_string(i));
		if (rule) {
			ImGui::PushStyleColor(ImGuiCol_Button, deadColor);
			ImGui::PushStyleColor(ImGuiCol_Text,   liveColor);
		} else {
			ImGui::PushStyleColor(ImGuiCol_Button, liveColor);
			ImGui::PushStyleColor(ImGuiCol_Text,   deadColor);
		}
		newDeathRules ^= (ImGui::Button(label.c_str(), ImVec2(ruleBoxWidth, 0)) ? 1 : 0) << i;
		ImGui::PopStyleColor(2);
		if (i < 8)
			ImGui::SameLine(0.0f, 0.0f);
	}

	if (deathRules != newDeathRules)
		mSimulator.setDeathRules(newDeathRules);

	LLCA2DSimulator::cell numGenerations = mSimulator.getNumGenerations();
	if (ImGui::DragInt("##NumGenerations", (int*)&numGenerations, 1, 1, 1000))
		setNumGenerations(numGenerations);

	ImGui::Text("Randomize:");
	if (ImGui::Button("Birth", ImVec2(ImGui::GetContentRegionAvail().x * (1.0f / 3.0f), 0.0f)))
		mSimulator.setBirthRules(mRandRule(mRD));
	ImGui::SameLine(0.0f, 0.0f);
	if (ImGui::Button("Death", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 0.0f)))
		mSimulator.setDeathRules(mRandRule(mRD));
	ImGui::SameLine(0.0f, 0.0f);
	if (ImGui::Button("Both", ImVec2(-FLT_MIN, 0.0f))) {
		mSimulator.setBirthRules(mRandRule(mRD));
		mSimulator.setDeathRules(mRandRule(mRD));
	}

	ImGui::Text("Border:");
	if (ImGui::ColorEdit4("##BorderColour", &mBorderColour[0])) {
		mShader.bind();
		glUniform4fv(mBorderColUniform, 1, &mBorderColour[0]);
		mShader.unbind();
	}

	ImGui::Text("Grid");
	if (ImGui::ColorEdit4("##GridColour", &mGridColour[0])) {
		mShader.bind();
		glUniform4fv(mGridColUniform, 1, &mGridColour[0]);
		mShader.unbind();
	}
}

void LLCA2DRenderer::drawImageConfig() {
	IconAtlasHandler::AtlasMap* map = IconAtlasHandler::getAtlasMap("Arrows");
	IconAtlasHandler::Icon& icon = map->icons.find("arrow_all [#328]")->second;
	if (ImGui::ImageButton("ResetView", (ImTextureID)map->atlas.id, ImVec2(16, 16), icon.uv1, icon.uv2))
		setViewOffset(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
}

void LLCA2DRenderer::focusAction() {
	ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
	if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
		mIsDragging = true;
	if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
		mIsDragging = false;
	if (mIsDragging) {
		ImVec2 mouseDelta = ImGui::GetIO().MouseDelta;
		glm::vec2 delta = 2.0f * glm::vec2(mouseDelta.x * mViewOffset.z / mImageBounds.z, mouseDelta.y * mViewOffset.w / mImageBounds.w);
		setViewOffset(mViewOffset - glm::vec4(delta, 0.0f, 0.0f));
	}

	if (ImGui::GetIO().MouseWheel != 0.0f) {
		glm::uvec2 bounds = getBounds();
		mZoomFactor = std::clamp((int)std::round(mZoomFactor - ImGui::GetIO().MouseWheel), (int)-std::sqrt(std::max(bounds.x, bounds.y)), cMaxZoom);
		float zoom = std::pow(1.5f, mZoomFactor);

		setViewOffset(glm::vec4(mViewOffset.x, mViewOffset.y, zoom, zoom));
	}
}

void LLCA2DRenderer::updateSim() {
	mSimulator.iterate();
}

void LLCA2DRenderer::drawSim() {
	mShader.bind();
	drawQuad();
	mShader.unbind();
}

void LLCA2DRenderer::imageResized() {
	mShader.bind();
	glUniform2fv(mImageBoundsUniform, 1, &mImageBounds[2]);
	mShader.unbind();
}
