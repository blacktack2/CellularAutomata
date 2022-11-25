#pragma once

class Simulator {
public:
	Simulator() = default;
	~Simulator() = default;

	virtual void reset() = 0;
	virtual void iterate() = 0;
};
