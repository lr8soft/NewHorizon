#pragma once
#ifndef _TIMER_H_
#define _TIMER_H_
class Timer {
private:
	long double accumulateTime = 0.0;
	float nowFrame = 0.0f, deltaFrame = 0.0f, lastTime = 0.0;

	bool FirstRun = true, EffectByPause = true;

	static bool IsPause, afterPause;
public:
	Timer(bool willEffectByPause = true);

	void Tick();
	void Tick(float nowFrame);
	void Clear();

	static void Pause();
	static void Resume();
	static void AfterResume();
	static bool getIsPause();

	float getNowFrame();
	float getDeltaFrame();
	float getLastFrame();

	long double getAccumlateTime();

	
};
#endif