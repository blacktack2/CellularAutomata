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
	virtual void drawImageConfig() override;

	virtual void focusAction() override;

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

	inline void setNumGenerations(LLCA2DSimulator::cell numGenerations) {
		mSimulator.setNumGenerations(numGenerations);
		mShader.bind();
		glUniform1f(mInvNumGenerationsUniform, 1.0f / numGenerations);
		mShader.unbind();
	}

	inline void setViewOffset(glm::vec4 viewOffset) {
		mViewOffset = viewOffset;
		mShader.bind();
		glUniform4fv(mViewOffsetUniform, 1, &mViewOffset[0]);
		mShader.unbind();
	}
protected:
	virtual void updateSim() override;
	virtual void drawSim() override;
	virtual void imageResized() override;
private:
	LLCA2DSimulator mSimulator;

	Shader mShader;

	GLuint mViewOffsetUniform;
	GLuint mSimBoundsUniform;
	GLuint mImageBoundsUniform;
	GLuint mInvNumGenerationsUniform;

	GLuint mLiveColUniform;
	GLuint mDeadColUniform;
	GLuint mBorderColUniform;
	GLuint mGridColUniform;

	glm::vec4 mViewOffset = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	int mZoomFactor = 0;

	glm::vec3 mLiveColour = glm::vec3(1.0f);
	glm::vec3 mDeadColour = glm::vec3(0.0f);
	glm::vec4 mBorderColour = glm::vec4(glm::vec3(0.4f), 0.0f);
	glm::vec4 mGridColour = glm::vec4(glm::vec3(0.5f), 0.0f);

	bool mIsDragging = false;

	const int cMaxZoom = 3;

	std::random_device mRD;
	std::mt19937 mMT = std::mt19937(mRD());
	std::uniform_int_distribution<LLCA2DSimulator::ruleset> mRandRule = std::uniform_int_distribution<LLCA2DSimulator::ruleset>(0, (1 << 9) - 1);
};