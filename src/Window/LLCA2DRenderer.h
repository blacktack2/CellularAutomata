#pragma once
#include "Renderer.h"
#include "Shader.h"
#include "../Simulation/LLCA2DSimulator.h"

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

class LLCA2DRenderer : public Renderer {
public:
	LLCA2DRenderer(Window& window, GLuint& texture);
	~LLCA2DRenderer();

	virtual void drawParameters() override;

	virtual void updateSim() override;
	virtual void drawSim() override;
private:
	ruleset parseRuleset(char* rules);

	LLCA2DSimulator mSimulator;

	Shader mShader;

	GLuint mSimBoundsUniform;
	GLuint mInvNumGenerationsUniform;

	GLuint mLiveColUniform;
	GLuint mDeadColUniform;

	glm::vec3 mCurrentLiveColor = glm::vec3(1.0f);
	glm::vec3 mCurrentDeadColor = glm::vec3(0.0f);
};