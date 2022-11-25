#include "LLCA2DSimulator.h"

#include "GLUtils.h"

const GLchar* iterationComputeCode =
#include "../Shaders/LLCA2D.comp"
;

LLCA2DSimulator::LLCA2DSimulator() : Simulator(), mIterationCompute(&iterationComputeCode) {
	mIterationCompute.bind();

	mBirthRulesUniform = glGetUniformLocation(mIterationCompute.getProgram(), "birthRules");
	mDeathRulesUniform = glGetUniformLocation(mIterationCompute.getProgram(), "deathRules");
	mNumGenerationsUniform = glGetUniformLocation(mIterationCompute.getProgram(), "numGenerations");
	mBoundsUniform = glGetUniformLocation(mIterationCompute.getProgram(), "simulationBounds");

	mEdgeModeUniform = glGetUniformLocation(mIterationCompute.getProgram(), "edgeMode");

	glUniform1ui(mBirthRulesUniform, mBirthRules);
	glUniform1ui(mDeathRulesUniform, mDeathRules);
	glUniform1ui(mNumGenerationsUniform, mNumGenerations);
	glUniform2ui(mBoundsUniform, mBounds.x, mBounds.y);

	glUniform1ui(mEdgeModeUniform, (unsigned int)mEdgeMode);

	mIterationCompute.unbind();

#ifdef _DEBUG
	glCheckError();
#endif

	mCellsBufferA = createBuffer(mCells, sizeof(mCells), 1);
	mCellsBufferB = createBuffer(mCells, sizeof(mCells), 2);

	reset();
}

LLCA2DSimulator::~LLCA2DSimulator() {
}

void LLCA2DSimulator::reset() {
	for (auto& c : mCells)
		c = mRandBool(mMT) ? mNumGenerations : 0;
	writeBuffer(mCellsBufferA, mCells, sizeof(mCells));
	writeBuffer(mCellsBufferB, mCells, sizeof(mCells));
}

void LLCA2DSimulator::iterate() {
	mIterationCompute.run(mBounds.x, mBounds.y, 1);
	swapBuffers(mCellsBufferA, mCellsBufferB, 1, 2);
}

void LLCA2DSimulator::updateGenerations(cell newGen, cell oldGen) {
	readBuffer(mCellsBufferB, mCells, sizeof(mCells));
	for (auto& c : mCells)
		c = c == oldGen ? newGen : c;
	writeBuffer(mCellsBufferA, mCells, sizeof(mCells));
	writeBuffer(mCellsBufferB, mCells, sizeof(mCells));
}
