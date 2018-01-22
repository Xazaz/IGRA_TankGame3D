#include "stdafx.h"
#include "Clock.h"
#include "RobotArm.h"

Clock::Clock()
{
	
}
__int64 startTimeInCounts = 0;
__int64 lastTimeInCounts = 0;
__int64 countsPerSecond = 5;

void Clock::Start() {
	__int64 currentTimeInCounts;
	// countsPerSecond depends on your PC
	if (!QueryPerformanceFrequency(
		(LARGE_INTEGER *)&countsPerSecond)) {
		MessageBox(NULL, L"QueryPerformanceFrequency Failed.", L"ERROR", MB_OK | MB_ICONINFORMATION);
		return;
	}
	QueryPerformanceCounter(
		(LARGE_INTEGER *)&currentTimeInCounts);
	startTimeInCounts = currentTimeInCounts;
	previousTimeInCounts = currentTimeInCounts;
}

double Clock::TimePassedSincePreviousTime() {
	__int64 currentTimeInCounts, timePassedSinceLastTimeInCounts;
	// Calculate time passed in seconds since last call to
	// GetTimePassedSinceLastTime
	QueryPerformanceCounter((LARGE_INTEGER *)&currentTimeInCounts);
	timePassedSinceLastTimeInCounts =
		currentTimeInCounts - previousTimeInCounts;
	double timePassedSinceLastTimeInSeconds =
		(currentTimeInCounts - previousTimeInCounts) /
		(double)countsPerSecond;
	previousTimeInCounts = currentTimeInCounts;
	return timePassedSinceLastTimeInSeconds;
}

double Clock::TimePassedSinceStartTime() {
	__int64 currentTimeInCounts;
	double timePassedSeconds;
	// Calculate time passed in seconds since timer was started
	QueryPerformanceCounter((LARGE_INTEGER *)&currentTimeInCounts);
	timePassedSeconds = (currentTimeInCounts - startTimeInCounts) /
		(double)countsPerSecond;

	return timePassedSeconds;
}
// Return relative time in seconds - since last measurement
double Clock::GetTimePassedSinceLastTime() {
	__int64 currentTimeInCounts, timePassedSinceLastTimeInCounts;
	// Calculate time passed in seconds since last call to
	// GetTimePassedSinceLastTime
	QueryPerformanceCounter((LARGE_INTEGER *)&currentTimeInCounts);
	timePassedSinceLastTimeInCounts = currentTimeInCounts - lastTimeInCounts;
	double timePassedSinceLastTimeInSeconds =
		(currentTimeInCounts - lastTimeInCounts) /
		(double)countsPerSecond;
	lastTimeInCounts = currentTimeInCounts;
	return timePassedSinceLastTimeInSeconds;
}

Clock::~Clock()
{
}
