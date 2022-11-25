#include "Timer.h"

std::chrono::milliseconds Timer::getFrameDelta() {
	auto currentTime = getCurrentTime();
	auto d = currentTime - mLastTime;
	mDelta += d.count();
	mFrameCounter++;
	if (mDelta > 1000) {
		mMs = (float)mDelta / (float)mFrameCounter;
		mFps = 1000.0f / mMs;
		mDelta -= 1000;
		mFrameCounter = 0;
	}
	mLastTime = currentTime;
	return d;
}

Timer::Timer() :
mLastTime(getCurrentTime()) {

}

Timer::~Timer() {
}
