#include "LLCA2DSerializer.h"

LLCA2DSerializer::LLCA2DSerializer(LLCA2DRenderer& renderer, LLCA2DSimulator& simulator) :
Serializer(), mRenderer(renderer), mSimulator(simulator) {
}

bool LLCA2DSerializer::read(const std::string& filename) {
	Data data;
	if (!read_(filename.c_str(), (void*)&data, sizeof(Data), 1))
		return false;

	mRenderer.setLiveColour(data.liveColour);
	mRenderer.setDeadColour(data.deadColour);
	
	mRenderer.setBounds(data.bounds);

	mSimulator.setBirthRules(data.birthRules);
	mSimulator.setDeathRules(data.deathRules);
	mRenderer.setNumGenerations(data.numGenerations);

	mSimulator.setInitMode(data.initMode);
	mSimulator.setEdgeMode(data.edgeMode);

	mSimulator.reset();

	return true;
}

bool LLCA2DSerializer::write(const std::string& filename) {
	Data data{
		mRenderer.getLiveColour(),
		mRenderer.getDeadColour(),

		mRenderer.getBounds(),

		mSimulator.getBirthRules(),
		mSimulator.getDeathRules(),
		mSimulator.getNumGenerations(),

		mSimulator.getInitMode(),
		mSimulator.getEdgeMode()
	};

	return !write_(filename.c_str(), (void*)&data, sizeof(Data), 1);
}
