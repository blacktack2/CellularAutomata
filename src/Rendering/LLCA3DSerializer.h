#pragma once
#include "src/Window/Serializer.h"
#include "LLCA3DRenderer.h"
#include "src/Simulation/LLCA3DSimulator.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class LLCA3DSerializer : public Serializer {
public:
	LLCA3DSerializer(LLCA3DRenderer& renderer, LLCA3DSimulator& simulator);

	virtual bool read(const std::string& filename) override;
	virtual bool write(const std::string& filename) override;

	virtual const char* getFileExtension() override {
		return "llca3d";
	}
private:
	struct Data {
		glm::vec3 liveColour;
		glm::vec3 deadColour;

		glm::uvec3 bounds;

		LLCA3DSimulator::ruleset birthRules;
		LLCA3DSimulator::ruleset deathRules;
		LLCA3DSimulator::cell numGenerations;

		LLCA3DSimulator::InitMode initMode;
		LLCA3DSimulator::EdgeMode edgeMode;
	};

	LLCA3DRenderer& mRenderer;
	LLCA3DSimulator& mSimulator;
};