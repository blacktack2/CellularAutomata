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

	inline glm::vec3 getLiveColour() { return mLiveColour; }
	inline void setLiveColour(glm::vec3 colour) {
		mLiveColour = colour;
		mShader.bind();
		glUniform3fv(mLiveColUniform, 1, &mLiveColour[0]);
		mShader.unbind();
	}

	inline glm::vec3 getDeadColour() { return mDeadColour; }
	inline void setDeadColour(glm::vec3 colour) {
		mDeadColour = colour;
		mShader.bind();
		glUniform3fv(mDeadColUniform, 1, &mDeadColour[0]);
		mShader.unbind();
	}

	inline glm::uvec2 getBounds() { return mSimulator.getBounds(); }
	inline void setBounds(glm::uvec2 bounds) {
		mSimulator.setBounds(bounds);
		mShader.bind();
		glUniform2uiv(mSimBoundsUniform, 1, &bounds[0]);
		mShader.unbind();
	}

	inline void setNumGenerations(cell numGenerations) {
		mSimulator.setNumGenerations(numGenerations);
		mShader.bind();
		glUniform1f(mInvNumGenerationsUniform, 1.0f / numGenerations);
		mShader.unbind();
	}
private:
	LLCA2DSimulator mSimulator;

	Shader mShader;

	GLuint mSimBoundsUniform;
	GLuint mInvNumGenerationsUniform;

	GLuint mLiveColUniform;
	GLuint mDeadColUniform;

	glm::vec3 mLiveColour = glm::vec3(1.0f);
	glm::vec3 mDeadColour = glm::vec3(0.0f);
};