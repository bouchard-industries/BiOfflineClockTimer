// Copyright 2018 Bouchard Industries Corp
//
// Created by Eric Bouchard on 2018-04-14.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
// You may obtain a copy of the License at: http ://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" 
// BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License.

#include "BiOfflineClockTimer.h"
#include "BiUtils.h"

static const int FONT_SIZE = 15;

BiOfflineClockTimer::BiOfflineClockTimer(int currentEpochTime, int MaxEpochTime, int clockId, int amountOfLoops) :
    m_clockId(clockId),
    m_clockEpochTime(currentEpochTime),
    m_clockHours(0),
    m_clockMinutes(0),
    m_clockSeconds(0),
    m_maxEpochTime(MaxEpochTime),
    m_clockLabel(NULL),
    m_amountOfLoops(amountOfLoops)
{
    // Cocos2d-x code to create label for displaying the time left.
    m_clockLabel = Label::createWithTTF("00:00:00", "fonts/arial.ttf", FONT_SIZE);
    m_clockLabel->setPosition(Vec2(0, 0));
    this->addChild(m_clockLabel, 1);

    ResetClock(0, 0, 0);
}

BiOfflineClockTimer::~BiOfflineClockTimer()
{
}

void BiOfflineClockTimer::StartClock(bool bLoadSaveTime)
{
    // Cocos2d-x code. pUserDefault is used to save and load locally.
    UserDefault *pUserDefault = UserDefault::getInstance();

    std::string clockSaveLastEpochTimeKey = "clock" + ToString(m_clockId) + "lastEpochTime";

    // Cocos2d-x code. Loads int from local file.
    int clockLastSavedEpochTime = pUserDefault->getIntegerForKey(clockSaveLastEpochTimeKey.c_str());

    if (clockLastSavedEpochTime > 0 && bLoadSaveTime)
    {
        m_clockEpochTime = clockLastSavedEpochTime;
    }
    else
    {
        m_clockEpochTime = time(0);
    }

    std::string clockSavedMaxEpochTimeKey = "clock" + ToString(m_clockId) + "maxEpochTime";

    int clockSaveMaxEpochTime = pUserDefault->getIntegerForKey(clockSavedMaxEpochTimeKey.c_str());

    if (clockSaveMaxEpochTime > 0 && bLoadSaveTime)
    {
        m_maxEpochTime = clockSaveMaxEpochTime;
    }
    else
    {
        // Cocos2d-x code. Saves int to local file.
        pUserDefault->setIntegerForKey(clockSavedMaxEpochTimeKey.c_str(), m_maxEpochTime);
        pUserDefault->flush();
    }

    // Cocos2d-x code. Schedule ClockTick to tick every frame.
    schedule(schedule_selector(BiOfflineClockTimer::ClockTick));
}

void BiOfflineClockTimer::StopClock(bool bClearSaveTime)
{
    unschedule(schedule_selector(BiOfflineClockTimer::ClockTick));

    if (bClearSaveTime)
    {
        UserDefault *pUserDefault = UserDefault::getInstance();
        std::string clockSaveLastEpochTimeString = "clock" + ToString(m_clockId) + "lastEpochTime";
        pUserDefault->setIntegerForKey(clockSaveLastEpochTimeString.c_str(), 0);
        std::string clockSaveMaxEpochTimeString = "clock" + ToString(m_clockId) + "maxEpochTime";
        pUserDefault->setIntegerForKey(clockSaveMaxEpochTimeString.c_str(), 0);
        pUserDefault->flush();
    }

    ResetClock(0, 0, 0);
}

void BiOfflineClockTimer::ClockTick(float dt)
{
    int currentEpochTime = time(0);
    int timePassed = currentEpochTime - m_clockEpochTime;

    int clockEpochTimeDiff = m_maxEpochTime - timePassed;
    if (clockEpochTimeDiff < 0)
    {
        int timesLooped = timePassed / m_maxEpochTime;

        if (timesLooped < 0)
        {
            timesLooped *= -1;
        }

        timesLooped = timesLooped <= m_amountOfLoops ? timesLooped : m_amountOfLoops;

        for (int i = 0; i < timesLooped; i++)
        {
            // Cocos2d-x code. Sends a signal for each completed loop.
            EventCustom eventClockLooped(EVENT_CUSTOM_CLOCK_LOOPED);
            int clockIdStringLength = ToString(m_clockId).length() + 1;
            char *pBufClockIdStrLength = new char[clockIdStringLength];
            sprintf(pBufClockIdStrLength, "%d", m_clockId);
            eventClockLooped.setUserData(pBufClockIdStrLength);
            _eventDispatcher->dispatchEvent(&eventClockLooped);
            CC_SAFE_DELETE_ARRAY(pBufClockIdStrLength);
        }
		
        ResetClock(0, 0, 0);
        clockEpochTimeDiff = (timePassed - (m_maxEpochTime * timesLooped));
        m_clockEpochTime -= clockEpochTimeDiff;
        clockEpochTimeDiff = m_maxEpochTime - (currentEpochTime - m_clockEpochTime);
        return;
    }

    int oldClockSecond = m_clockSeconds;

    m_clockSeconds = clockEpochTimeDiff % 60;
    m_clockMinutes = (clockEpochTimeDiff / 60) % 60;
    m_clockHours = clockEpochTimeDiff / 3600;

    if (oldClockSecond != m_clockSeconds)
    {
        UserDefault *pUserDefault = UserDefault::getInstance();
        std::string clockSaveLastEpochTimeString = "clock" + ToString(m_clockId) + "lastEpochTime";
        pUserDefault->setIntegerForKey(clockSaveLastEpochTimeString.c_str(), m_clockEpochTime);
        pUserDefault->flush();
    }

    UpdateLabel();
}

void BiOfflineClockTimer::UpdateLabel()
{
    std::string stringTime = m_clockHours > 0 ? ToString(m_clockHours) + ":" : "";

    if (m_clockMinutes > 9)
    {
        stringTime += ToString(m_clockMinutes) + ":";
    }
    else if (m_clockMinutes > 0)
    {
        stringTime += "0" + ToString(m_clockMinutes) + ":";
    }
    else
    {
        stringTime += "00:";
    }

    if (m_clockSeconds > 9)
    {
        stringTime += ToString(m_clockSeconds);
    }
    else if (m_clockSeconds > 0)
    {
        stringTime += "0" + ToString(m_clockSeconds);
    }
    else
    {
        stringTime += "00";
    }

    m_clockLabel->setString(stringTime);
}

void BiOfflineClockTimer::ResetClock(int clockHour, int clockMinute, int clockSecond)
{
    m_clockEpochTime = time(0);

    m_clockSeconds = m_maxEpochTime % 60;
    m_clockMinutes = (m_maxEpochTime / 60) % 60;
    m_clockHours = m_maxEpochTime / 3600;

    UpdateLabel();
}

bool BiOfflineClockTimer::IsClockTicking()
{
    // Cocos2d-x code.
    return getScheduler()->isScheduled(schedule_selector(BiOfflineClockTimer::ClockTick), this);
}

int BiOfflineClockTimer::GetEpochTimeLeft()
{
    int currentEpochTime = time(0);
    int timePassed = currentEpochTime - m_clockEpochTime;
    return (m_maxEpochTime - timePassed);
}

int BiOfflineClockTimer::GetMaxEpochTime()
{
    return m_maxEpochTime;
}

std::string BiOfflineClockTimer::ToString(int value)
{
    std::ostringstream os;
    os << value;
    return os.str();
}
