#pragma once
#include "ComputeShader.h"
#include "Simulator.h"

#include <glm/vec2.hpp>

#include <random>

class LLCA3DSimulator : public Simulator {
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
		LINEX,
		LINEY,
		LINEZ,
		CROSS,
		MAX
	};

	LLCA3DSimulator();
	~LLCA3DSimulator();

	virtual void reset() override;
	virtual void iterate() override;

	static const size_t cMaxSize = 100;
	static const size_t cMaxCells = cMaxSize * cMaxSize * cMaxSize;
private:
	void updateGenerations(cell newGen, cell oldGen);

	void clearCells();
	void fillCellsRandom();
	void fillCells1();
	void fillCellsLineH();
	void fillCellsLineV();
	void fillCellsCross();

	cell mCells[cMaxCells]{};

	GLuint mCellsBufferA;
	GLuint mCellsBufferB;
};
