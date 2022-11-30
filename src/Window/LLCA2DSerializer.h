#pragma once
#include "Serializer.h"
#include "LLCA2DRenderer.h"
#include "../Simulation/LLCA2DSimulator.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class LLCA2DSerializer : public Serializer {
public:
	LLCA2DSerializer(LLCA2DRenderer& renderer, LLCA2DSimulator& simulator);

	virtual bool read(const std::string& filename) override;
	virtual bool write(const std::string& filename) override;

	virtual const char* getFileExtension() override {
		return "llca2d";
	}
private:
	struct Data {
		glm::vec3 liveColour;
		glm::vec3 deadColour;

		glm::uvec2 bounds;

		ruleset birthRules;
		ruleset deathRules;
		cell numGenerations;

		InitMode initMode;
		EdgeMode edgeMode;
	};

	LLCA2DRenderer& mRenderer;
	LLCA2DSimulator& mSimulator;
};