#include "Timer.h"
#include <glfw/glfw3.h>
#include <iostream>
bool Timer::IsPause = false;
bool Timer::afterPause = false;
Timer::Timer(bool willEffectByPause)
{
	EffectByPause = willEffectByPause;
}
void Timer::Tick()
{
	if (!IsPause || !EffectByPause)
	{
		if (afterPause && EffectByPause)
		{
			nowFrame = glfwGetTime();
			lastTime = nowFrame;
			deltaFrame = 0.0f;
			afterPause = false;
			return;
		}
		if (!FirstRun) {//Not first run
			nowFrame = glfwGetTime();
			deltaFrame = nowFrame - lastTime;
			lastTime += deltaFrame;
		}
		else {
			nowFrame = glfwGetTime();
			deltaFrame = 0;
			lastTime = nowFrame;
			lastFpsUpdate = lastTime;
			FirstRun = false;
		}
		frameCounter++;
		if (lastTime - lastFpsUpdate >= 1) {
			lastFpsUpdate = lastTime;
			fpsNow = frameCounter;
			frameCounter = 0;
		}
		accumulateTime += deltaFrame;
	}
}

void Timer::Tick(float update_nowFrame)
{
	if (!IsPause || !EffectByPause)
	{
		if (afterPause && EffectByPause)
		{
			nowFrame = update_nowFrame;
			lastTime = update_nowFrame;
			deltaFrame = 0.0f;
			return;
		}
		if (!FirstRun) {//Not first run
			nowFrame = update_nowFrame;
			deltaFrame = nowFrame - lastTime;
			lastTime += deltaFrame;
		}
		else {
			nowFrame = update_nowFrame;
			deltaFrame = 0;
			lastTime = nowFrame;
			lastFpsUpdate = lastTime;
			FirstRun = false;
		}
		frameCounter++;
		if (lastTime - lastFpsUpdate >= 1) {
			lastFpsUpdate = lastTime;
			fpsNow = frameCounter;
			frameCounter = 0;
		}
		accumulateTime += deltaFrame;
	}
}


void Timer::Clear()
{
	fpsNow = 0.0f;
	frameCounter = 0.0f;
	nowFrame = 0.0f;
	deltaFrame = 0.0f;
	lastTime = 0.0f;
	lastFpsUpdate = 0.0f;
	accumulateTime = 0.0;
	FirstRun = true;
}

void Timer::Pause()
{
	IsPause = true;
}

void Timer::Resume()
{
	IsPause = false;
	afterPause = true;
}

void Timer::AfterResume()
{
	afterPause = false;
}

float Timer::getNowFrame()
{
	return nowFrame;
}

float Timer::getDeltaFrame()
{
	return deltaFrame;
}

float Timer::getLastFrame()
{
	return lastTime;
}

float Timer::getFramePerSecond()
{
	return fpsNow;
}

long double Timer::getAccumlateTime()
{
	return accumulateTime;
}

bool Timer::getIsPause()
{
	return IsPause;
}
