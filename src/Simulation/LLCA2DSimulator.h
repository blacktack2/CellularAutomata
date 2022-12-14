#pragma once
#include "ComputeShader.h"
#include "Simulator.h"

#include <glm/vec2.hpp>

#include <random>

// 2D Lifelike Cellular Automata
class LLCA2DSimulator : public Simulator {
public:
	typedef unsigned int cell;
	typedef unsigned int ruleset;

	enum class EdgeMode {
		CLAMP = 0u,
		WRAP = 1u,
		DEAD = 2u,
		LIVE = 3u,
		MAX
	};

	enum class InitMode {
		RANDOM,
		SINGLE,
		LINEH,
		LINEV,
		CROSS,
		MAX
	};

	LLCA2DSimulator();
	~LLCA2DSimulator();

	virtual void reset() override;
	virtual void iterate() override;

	[[nodiscard]] inline ruleset getBirthRules() const {
		return mBirthRules;
	}
	inline void setBirthRules(ruleset birthRules) {
		mBirthRules = birthRules;
		mIterationCompute.bind();
		glUniform1ui(mBirthRulesUniform, mBirthRules);
		mIterationCompute.unbind();
	}

	[[nodiscard]] inline ruleset getDeathRules() const {
		return mDeathRules;
	}
	inline void setDeathRules(ruleset deathRules) {
		mDeathRules = deathRules;
		mIterationCompute.bind();
		glUniform1ui(mDeathRulesUniform, mDeathRules);
		mIterationCompute.unbind();
	}

	[[nodiscard]] inline glm::uvec2 getBounds() const {
		return mBounds;
	}
	inline void setBounds(glm::uvec2 bounds) {
		mBounds = bounds;
		mIterationCompute.bind();
		glUniform2ui(mBoundsUniform, mBounds.x, mBounds.y);
		mIterationCompute.unbind();
	}

	[[nodiscard]] inline cell getNumGenerations() const {
		return mNumGenerations;
	}
	inline void setNumGenerations(cell numGenerations) {
		updateGenerations(numGenerations, mNumGenerations);
		mNumGenerations = numGenerations;
		mIterationCompute.bind();
		glUniform1ui(mNumGenerationsUniform, mNumGenerations);
		mIterationCompute.unbind();
	}

	[[nodiscard]] inline EdgeMode getEdgeMode() const {
		return mEdgeMode;
	}
	inline void setEdgeMode(EdgeMode edgeMode) {
		mEdgeMode = edgeMode;
		mIterationCompute.bind();
		glUniform1ui(mEdgeModeUniform, (unsigned int)mEdgeMode);
		mIterationCompute.unbind();
	}

	[[nodiscard]] inline InitMode getInitMode() const {
	    return mInitMode;
	}
	inline void setInitMode(InitMode initMode) {
	    mInitMode = initMode;
	}

	static const size_t cMaxSize = 1000;
	static const size_t cMaxCells = cMaxSize * cMaxSize;
private:
	void updateGenerations(cell newGen, cell oldGen);

	void clearCells();
	void fillCellsRandom();
	void fillCells1();
	void fillCellsLineH();
	void fillCellsLineV();
	void fillCellsCross();

	ComputeShader mIterationCompute;

	ruleset mBirthRules = 1u << 3u;
	ruleset mDeathRules = ~(1u << 2u | 1u << 3u) & ((1u << 10u) - 1u);
	glm::uvec2 mBounds = glm::uvec2(1000u, 1000u);
	cell mNumGenerations = 1u;

	EdgeMode mEdgeMode = EdgeMode::WRAP;
	InitMode mInitMode = InitMode::RANDOM;

	GLuint mBirthRulesUniform;
	GLuint mDeathRulesUniform;
	GLuint mBoundsUniform;
	GLuint mNumGenerationsUniform;

	GLuint mEdgeModeUniform;

	cell mCells[cMaxCells]{};

	GLuint mCellsBufferA;
	GLuint mCellsBufferB;

	std::random_device mRD;
	std::mt19937 mMT = std::mt19937(mRD());
	std::uniform_int_distribution<int> mRandBool = std::uniform_int_distribution<int>(0, 1);
};
