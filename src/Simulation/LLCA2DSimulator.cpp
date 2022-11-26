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

	LLCA2DSimulator::reset();
}

LLCA2DSimulator::~LLCA2DSimulator() {
}

void LLCA2DSimulator::reset() {
    clearCells();
    switch (mInitMode) {
        default: case InitMode::MAX: break;
        case InitMode::RANDOM: fillCellsRandom(); break;
        case InitMode::SINGLE: fillCells1(); break;
        case InitMode::LINEH: fillCellsLineH(); break;
        case InitMode::LINEV: fillCellsLineV(); break;
        case InitMode::CROSS: fillCellsCross(); break;
    }
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

void LLCA2DSimulator::clearCells() {
    for (auto& c : mCells)
        c = 0;
}

void LLCA2DSimulator::fillCellsRandom() {
    for (size_t x = 0; x < mBounds.x; x++)
        for (size_t y = 0; y < mBounds.y; y++)
            mCells[x + y * MAX_SIZE] = mRandBool(mMT) ? mNumGenerations : 0;
}

void LLCA2DSimulator::fillCells1() {
    mCells[(size_t)(mBounds.x * 0.5f + mBounds.y * 0.5f * MAX_SIZE)] = mNumGenerations;
}

void LLCA2DSimulator::fillCellsLineH() {
    for (size_t x = 0; x < mBounds.x; x++)
        mCells[x + (size_t)(mBounds.y * 0.5f * MAX_SIZE)] = mNumGenerations;
}

void LLCA2DSimulator::fillCellsLineV() {
    for (size_t y = 0; y < mBounds.y; y++)
        mCells[(size_t)(mBounds.x * 0.5f) + y * MAX_SIZE] = mNumGenerations;
}

void LLCA2DSimulator::fillCellsCross() {
    for (size_t x = 0; x < mBounds.x; x++)
        mCells[x + (size_t)(mBounds.y * 0.5f * MAX_SIZE)] = mNumGenerations;
    for (size_t y = 0; y < mBounds.y; y++)
        mCells[(size_t)(mBounds.x * 0.5f) + y * MAX_SIZE] = mNumGenerations;
}
