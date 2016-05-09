#include "Timer.hpp"

Timer::Timer()
{
    alreadyStarted = false;
}

void Timer::set(int _newMillis)
{
    totalTime = _newMillis;
    alreadyStarted = false;
}

// Starting the timer
void Timer::start()
{
    savedTime = ofGetElapsedTimeMillis();
    alreadyStarted = true;
}

void Timer::reset(int newMillis)
{
    savedTime = ofGetElapsedTimeMillis();
    alreadyStarted = true;
    totalTime = newMillis;
}

bool Timer::finished()
{
    // Check how much time has passed
    int passedTime = ofGetElapsedTimeMillis()- savedTime;
    if (passedTime > totalTime && alreadyStarted)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Timer::hasStarted()
{
    return alreadyStarted;
}
