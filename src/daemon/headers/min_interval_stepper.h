#pragma once

#include <time.h>
#include <stdexcept>
#include <signal.h>

/**
 * @brief waits just long enough to make a step take as long as it should, taking into account the time already spent on it
 */
class MinIntervalStepper {
    timespec lastTime = {0,0};
    timespec interval;
    static const long NS_IN_A_SEC = 1000000000;

    static inline void carryUpWrapArounds(timespec & t);
    static inline timespec addTime(const timespec & a, const timespec & b);
    static inline timespec subtractTime(const timespec & a, const timespec & b);
    static inline timespec getTime();

    public:
        /**
         * @brief Construct a new Min Interval Stepper object
         * @param minIntervalMs default is zero, resulting in no waiting between steps.
         */
        MinIntervalStepper(int minIntervalMs = 0);

        /**
         * @brief waits until minimum time has passed since last step, unless an interupt occurs
         * 
         * @return true if step completed, false if interupted by signal
         */
        bool step();
};