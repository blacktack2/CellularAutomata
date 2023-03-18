#include "LLCA3DSimulator.h"

#include "src/Rendering/GLUtils.h"

const GLchar* LLCA3Dcomp =
#include "../Shaders/LLCA3D.comp"
;

LLCA3DSimulator::LLCA3DSimulator() : Simulator(), mIterationCompute(&LLCA3Dcomp) {
	mIterationCompute.bind();

	mBirthRulesUniform = glGetUniformLocation(mIterationCompute.getProgram(), "birthRules");
	mDeathRulesUniform = glGetUniformLocation(mIterationCompute.getProgram(), "deathRules");
	mNumGenerationsUniform = glGetUniformLocation(mIterationCompute.getProgram(), "numGenerations");
	mBoundsUniform = glGetUniformLocation(mIterationCompute.getProgram(), "simulationBounds");

	mEdgeModeUniform = glGetUniformLocation(mIterationCompute.getProgram(), "edgeMode");

	glUniform1ui(mBirthRulesUniform, mBirthRules);
	glUniform1ui(mDeathRulesUniform, mDeathRules);
	glUniform1ui(mNumGenerationsUniform, mNumGenerations);
	glUniform3uiv(mBoundsUniform, 1, &mBounds[0]);

	glUniform1ui(mEdgeModeUniform, (GLuint)mEdgeMode);

	mIterationCompute.unbind();

#ifdef _DEBUG
	glCheckError();
#endif

	mCellsBufferA = createBuffer(mCells, sizeof(mCells), 1);
	mCellsBufferB = createBuffer(mCells, sizeof(mCells), 2);

	LLCA3DSimulator::reset();
}

LLCA3DSimulator::~LLCA3DSimulator() {
}

void LLCA3DSimulator::reset() {
	clearCells();
	switch (mInitMode) {
		default: case InitMode::MAX: break;
		case InitMode::RANDOM: fillCellsRandom(); break;
		case InitMode::SINGLE: fillCells1(); break;
		case InitMode::LINEX: fillCellsLineX(); break;
		case InitMode::LINEY: fillCellsLineY(); break;
		case InitMode::LINEZ: fillCellsLineZ(); break;
		case InitMode::CROSS: fillCellsCross(); break;
	}
	writeBuffer(mCellsBufferA, mCells, sizeof(mCells));
	writeBuffer(mCellsBufferB, mCells, sizeof(mCells));
}

void LLCA3DSimulator::iterate() {
	mIterationCompute.run(mBounds.x, mBounds.y, mBounds.z);
	swapBuffers(mCellsBufferA, mCellsBufferB, 1, 2);
}

void LLCA3DSimulator::updateGenerations(cell newGen, cell oldGen) {
	readBuffer(mCellsBufferB, mCells, sizeof(mCells));
	for (auto& c : mCells)
		c = c == oldGen ? newGen : c;
	writeBuffer(mCellsBufferA, mCells, sizeof(mCells));
	writeBuffer(mCellsBufferB, mCells, sizeof(mCells));
}

void LLCA3DSimulator::clearCells() {
	for (auto& c : mCells)
		c = 0;
}

void LLCA3DSimulator::fillCellsRandom() {
	for (size_t x = 0; x < mBounds.x; x++)
		for (size_t y = 0; y < mBounds.y; y++)
			for (size_t z = 0; z < mBounds.z; z++)
				mCells[x + y * cMaxSize + z * cMaxSize * cMaxSize] = mRandBool(mMT) ? mNumGenerations : 0;
}

void LLCA3DSimulator::fillCells1() {
	mCells[(size_t)(mBounds.x * 0.5f + mBounds.y * 0.5f * cMaxSize + mBounds.z * 0.5f + cMaxSize * cMaxSize)] = mNumGenerations;
}

void LLCA3DSimulator::fillCellsLineX() {
	for (size_t x = 0; x < mBounds.x; x++)
		mCells[x + (size_t)(mBounds.y * 0.5f * cMaxSize + mBounds.z * 0.5f * cMaxSize * cMaxSize)] = mNumGenerations;
}

void LLCA3DSimulator::fillCellsLineY() {
	for (size_t y = 0; y < mBounds.y; y++)
		mCells[y * cMaxSize + (size_t)(mBounds.x * 0.5f + mBounds.z * 0.5f * cMaxSize * cMaxSize)] = mNumGenerations;
}

void LLCA3DSimulator::fillCellsLineZ() {
	for (size_t z = 0; z < mBounds.z; z++)
		mCells[z * cMaxSize * cMaxSize + (size_t)(mBounds.x * 0.5f + mBounds.y * 0.5f * cMaxSize)] = mNumGenerations;
}

void LLCA3DSimulator::fillCellsCross() {
	for (size_t x = 0; x < mBounds.x; x++)
		mCells[x + (size_t)(mBounds.y * 0.5f * cMaxSize + mBounds.z * 0.5f * cMaxSize * cMaxSize)] = mNumGenerations;
	for (size_t y = 0; y < mBounds.y; y++)
		mCells[y * cMaxSize + (size_t)(mBounds.x * 0.5f + mBounds.z * 0.5f * cMaxSize * cMaxSize)] = mNumGenerations;
	for (size_t z = 0; z < mBounds.z; z++)
		mCells[z * cMaxSize * cMaxSize + (size_t)(mBounds.x * 0.5f + mBounds.y * 0.5f * cMaxSize)] = mNumGenerations;
}
