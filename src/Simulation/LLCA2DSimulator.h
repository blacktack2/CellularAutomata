#pragma once
#include "ComputeShader.h"
#include "Simulator.h"

#include <glm/vec2.hpp>

#include <random>

const size_t MAX_SIZE = 2000;
const size_t MAX_CELLS = MAX_SIZE * MAX_SIZE;

typedef unsigned int cell;
typedef unsigned int ruleset;

enum class EdgeMode {
	CLAMP = 0u,
	WRAP = 1u,
	DEAD = 2u,
	LIVE = 3u,
	MAX
};

// 2D Lifelike Cellular Automata
class LLCA2DSimulator : public Simulator {
public:
	LLCA2DSimulator();
	~LLCA2DSimulator();

	virtual void reset() override;
	virtual void iterate() override;

	inline ruleset getBirthRules() {
		return mBirthRules;
	}
	inline void setBirthRules(ruleset birthRules) {
		mBirthRules = birthRules;
		mIterationCompute.bind();
		glUniform1ui(mBirthRulesUniform, mBirthRules);
		mIterationCompute.unbind();
	}

	inline ruleset getDeathRules() {
		return mDeathRules;
	}
	inline void setDeathRules(ruleset deathRules) {
		mDeathRules = deathRules;
		mIterationCompute.bind();
		glUniform1ui(mDeathRulesUniform, mDeathRules);
		mIterationCompute.unbind();
	}

	inline glm::uvec2 getBounds() {
		return mBounds;
	}
	inline void setBounds(glm::uvec2 bounds) {
		mBounds = bounds;
		mIterationCompute.bind();
		glUniform2ui(mBoundsUniform, mBounds.x, mBounds.y);
		mIterationCompute.unbind();
	}

	inline cell getNumGenerations() {
		return mNumGenerations;
	}
	inline void setNumGenerations(cell numGenerations) {
		updateGenerations(numGenerations, mNumGenerations);
		mNumGenerations = numGenerations;
		mIterationCompute.bind();
		glUniform1ui(mNumGenerationsUniform, mNumGenerations);
		mIterationCompute.unbind();
	}

	inline EdgeMode getEdgeMode() {
		return mEdgeMode;
	}
	inline void setEdgeMode(EdgeMode edgeMode) {
		mEdgeMode = edgeMode;
		mIterationCompute.bind();
		glUniform1ui(mEdgeModeUniform, (unsigned int)mEdgeMode);
		mIterationCompute.unbind();
	}
private:
	void updateGenerations(cell newGen, cell oldGen);

	ComputeShader mIterationCompute;

	ruleset mBirthRules = 1 << 3;
	ruleset mDeathRules = ~(1 << 2 | 1 << 3) & ((1 << 10) - 1);
	glm::uvec2 mBounds = glm::uvec2(100u, 100u);
	cell mNumGenerations = 1u;

	EdgeMode mEdgeMode = EdgeMode::WRAP;

	GLuint mBirthRulesUniform;
	GLuint mDeathRulesUniform;
	GLuint mBoundsUniform;
	GLuint mNumGenerationsUniform;

	GLuint mEdgeModeUniform;

	cell mCells[MAX_CELLS]{};

	GLuint mCellsBufferA;
	GLuint mCellsBufferB;

	std::random_device mRD;
	std::mt19937 mMT = std::mt19937(mRD());
	std::uniform_int_distribution<int> mRandBool = std::uniform_int_distribution<int>(0, 1);
};
