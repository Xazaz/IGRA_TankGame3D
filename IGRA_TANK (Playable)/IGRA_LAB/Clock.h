#pragma once
#ifndef CLOCK_H
#define CLOCK_H
#include "RobotArm.h"

class Clock
{
public:
	__int64 startTimeInCounts;
	__int64 previousTimeInCounts;
	__int64 countsPerSecond;

	Clock();
	void Start();
	double TimePassedSincePreviousTime();
	double TimePassedSinceStartTime();
	double GetTimePassedSinceLastTime();
	~Clock();
};
#endif
