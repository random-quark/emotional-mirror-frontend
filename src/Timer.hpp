#pragma once
#include "ofMain.h"

class Timer {
    public:
        Timer();

        int savedTime; // When Timer started
        int totalTime; // How long Timer should last
        bool alreadyStarted;

        void start();
        void set(int _newMillis);
        void reset(int newMillis);
        bool finished();
        bool hasStarted();
};
