#include "LLCA3DSerializer.h"

LLCA3DSerializer::LLCA3DSerializer(LLCA3DRenderer& renderer, LLCA3DSimulator& simulator) :
Serializer(), mRenderer(renderer), mSimulator(simulator) {
}

bool LLCA3DSerializer::read(const std::string& filename) {
	return false;
}

bool LLCA3DSerializer::write(const std::string& filename) {
	return false;
}
