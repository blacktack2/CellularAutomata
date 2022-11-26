#include "LLCA2DRenderer.h"

#include "Window.h"

#include <cmath>
#include <string>

const GLchar* shaderVertexCode =
#include "../Shaders/texQuad.vert"
;
const GLchar* shaderFragmentCode =
#include "../Shaders/LLCA2D.frag"
;

LLCA2DRenderer::LLCA2DRenderer(Window& window, GLuint& texture) :
mSimulator(), Renderer(window, mSimulator, texture), mShader(&shaderVertexCode, &shaderFragmentCode) {
	mShader.bind();
	
	mSimBoundsUniform  = glGetUniformLocation(mShader.getProgram(), "simulationBounds");
	mInvNumGenerationsUniform = glGetUniformLocation(mShader.getProgram(), "invNumGenerations");

	mLiveColUniform = glGetUniformLocation(mShader.getProgram(), "liveCol");
	mDeadColUniform = glGetUniformLocation(mShader.getProgram(), "deadCol");

	glUniform2uiv(mSimBoundsUniform, 1, &mSimulator.getBounds()[0]);
	glUniform1f(mInvNumGenerationsUniform, 1.0f / mSimulator.getNumGenerations());

	glUniform3fv(mLiveColUniform, 1, &mCurrentLiveColor[0]);
	glUniform3fv(mDeadColUniform, 1, &mCurrentDeadColor[0]);

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
	if (ImGui::DragInt2("##bounds", (int*)&simBounds[0], 10, 2, MAX_SIZE)) {
		mSimulator.setBounds(simBounds);
		mShader.bind();
		glUniform2uiv(mSimBoundsUniform, 1, &simBounds[0]);
		mShader.unbind();
	}

    ImGui::Text("Init Mode");
    const char* INIT_MODE_NAMES[]{ "Random", "Single", "Horizontal", "Vertical", "Cross" };
    InitMode initMode = mSimulator.getInitMode();
    if (ImGui::Combo("##InitMode", (int*)&initMode, INIT_MODE_NAMES, (int)InitMode::MAX)) {
        mSimulator.setInitMode(initMode);
    }

	ImGui::Text("Edge Mode");
	const char* EDGE_MODE_NAMES[]{ "Clamp", "Wrap", "Dead", "Live" };
	EdgeMode edgeMode = mSimulator.getEdgeMode();
	if (ImGui::Combo("##EdgeMode", (int*)&edgeMode, EDGE_MODE_NAMES, (int)EdgeMode::MAX)) {
		mSimulator.setEdgeMode(edgeMode);
	}

	ImGui::Text("Live Colour");
	if (ImGui::ColorEdit3("##LiveColour", (float*)&mCurrentLiveColor[0])) {
		mShader.bind();
		glUniform3fv(mLiveColUniform, 1, &mCurrentLiveColor[0]);
		mShader.unbind();
	}

	ImGui::Text("Dead Colour");
	if (ImGui::ColorEdit3("##DeadColour", (float*)&mCurrentDeadColor[0])) {
		mShader.bind();
		glUniform3fv(mDeadColUniform, 1, &mCurrentDeadColor[0]);
		mShader.unbind();
	}

	ImGui::Separator();

	float ruleBoxWidth = std::floor(bounds.x * (1.0f / 9.0f));
	ImVec4 liveColor = ImVec4(mCurrentLiveColor.r, mCurrentLiveColor.g, mCurrentLiveColor.b, 1.0f);
	ImVec4 deadColor = ImVec4(mCurrentDeadColor.r, mCurrentDeadColor.g, mCurrentDeadColor.b, 1.0f);

	ImGui::Text("Birth Rules");
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Whether a dead cell should become alive or stay dead with 'n' neighbours");
	ruleset birthRules = mSimulator.getBirthRules();
	ruleset newBirthRules = birthRules;
	
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
	ruleset deathRules = mSimulator.getDeathRules();
	ruleset newDeathRules = deathRules;

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

	cell numGenerations = mSimulator.getNumGenerations();
	if (ImGui::DragInt("##NumGenerations", (int*)&numGenerations, 1, 1, 1000)) {
		mSimulator.setNumGenerations(numGenerations);
		mShader.bind();
		glUniform1f(mInvNumGenerationsUniform, 1.0f / numGenerations);
		mShader.unbind();
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

ruleset LLCA2DRenderer::parseRuleset(char* rules) {
	return ruleset();
}
