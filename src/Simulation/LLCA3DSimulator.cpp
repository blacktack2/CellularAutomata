#include "LLCA3DSimulator.h"

const GLchar* LLCA3Dcomp =
""
//#include "../Shaders/LLCA3D.comp"
;

LLCA3DSimulator::LLCA3DSimulator() : Simulator(), mIterationCompute(&LLCA3Dcomp) {
}

LLCA3DSimulator::~LLCA3DSimulator() {
}

void LLCA3DSimulator::reset() {
}

void LLCA3DSimulator::iterate() {
}

void LLCA3DSimulator::updateGenerations(cell newGen, cell oldGen) {
}

void LLCA3DSimulator::clearCells() {
}

void LLCA3DSimulator::fillCellsRandom() {
}

void LLCA3DSimulator::fillCells1() {
}

void LLCA3DSimulator::fillCellsLineH() {
}

void LLCA3DSimulator::fillCellsLineV() {
}

void LLCA3DSimulator::fillCellsCross() {
}
