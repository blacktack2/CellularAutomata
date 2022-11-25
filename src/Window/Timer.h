#pragma once
#include <chrono>

class Timer {
public:
	std::chrono::milliseconds getFrameDelta();

	inline float getFPS() {
		return mFps;
	}
	inline float getMS() {
		return mMs;
	}

	static inline std::chrono::milliseconds getCurrentTime() {
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	}
	static inline Timer& getTimer() {
		static Timer timer = Timer();
		return timer;
	}
private:
	Timer();
	~Timer();

	std::chrono::milliseconds mLastTime;
	long long mDelta;

	int mFrameCounter = 0;
	float mFps = 0.0f;
	float mMs = 0.0f;
};