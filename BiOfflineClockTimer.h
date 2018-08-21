// Copyright 2018 Bouchard Industries Corp
//
// Created by Eric Bouchard on 2018-04-14.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
// You may obtain a copy of the License at: http ://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" 
// BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.See the License for the specific language governing 
// permissions and limitations under the License.
//
//
// BiOfflineClockTimer
//
// Created by Eric Bouchard on 2018-04-14.
// Description: Lets you create a decrementing clock that can loop as many times as you want, even while the application is closed. You can have multiple clocks
// with m_clockId.

#ifndef __BI_OFFLINE_CLOCK_TIMER_H__
#define __BI_OFFLINE_CLOCK_TIMER_H__

#include "Constants.h"

class BiOfflineClockTimer : public Node // Cocos2d-x code. Inherit from Node so this object can be added to the Scene Graph so the clock label can be visible.
{
public:
	BiOfflineClockTimer(int currentEpochTime, int MaxEpochTime, int clockId, int amountOfLoops);
	~BiOfflineClockTimer();
	void StartClock(bool bLoadSaveTime);
	void StopClock(bool bClearSaveTime);
	void ClockTick(float dt);
	bool IsClockTicking();
private:
	void UpdateLabel();
	void ResetClock(int clockHour, int clockMinute, int clockSecond);
	int GetEpochTimeLeft();
	int GetMaxEpochTime();
	std::string ToString(int value);
	int m_clockId;
	int m_clockEpochTime;
	int m_clockHours;
	int m_clockMinutes;
	int m_clockSeconds;
	Label *m_clockLabel;
	int m_maxEpochTime;
	int m_amountOfLoops;
};

#endif // __BI_OFFLINE_CLOCK_TIMER_H__ 
