#include "min_interval_stepper.h"
#include <time.h>
#include <math.h>  
#include <assert.h>

timespec MinIntervalStepper::getTime() {
    timespec t;
    if(clock_gettime(CLOCK_MONOTONIC, &t))
        throw std::runtime_error("clock_gettime() failed; errno:" + std::to_string(errno));
    return t;
}

void MinIntervalStepper::carryUpWrapArounds(timespec & t) {
    bool isNsecNegative = signbit(t.tv_nsec);
    t.tv_sec += (t.tv_nsec - NS_IN_A_SEC*isNsecNegative) / NS_IN_A_SEC;
    t.tv_nsec = NS_IN_A_SEC * isNsecNegative + 
                (t.tv_nsec % NS_IN_A_SEC);
}

timespec MinIntervalStepper::addTime(const timespec & a, const timespec & b) {
    timespec sum;
    sum.tv_sec = a.tv_sec + b.tv_sec;
    sum.tv_nsec = a.tv_nsec + b.tv_nsec;
    return sum;
}

timespec MinIntervalStepper::subtractTime(const timespec & a, const timespec & b) {
    timespec ans;
    ans.tv_sec = a.tv_sec - b.tv_sec;
    ans.tv_nsec = a.tv_nsec - b.tv_nsec;
    return ans;
}


MinIntervalStepper::MinIntervalStepper(int minIntervalMs) 
      : interval{minIntervalMs/1000, (minIntervalMs % 1000) * (NS_IN_A_SEC/1000)}, 
        lastTime(getTime()) {}

bool MinIntervalStepper::step() {
    timespec current = getTime();
    timespec elapsed = subtractTime(current, lastTime);
    timespec remaining = subtractTime(interval, elapsed);

    //need this to actually become a usable value
    carryUpWrapArounds(remaining);

    assert(remaining.tv_nsec >= 0 && remaining.tv_nsec < NS_IN_A_SEC);
    
    if (remaining.tv_sec < 0) {
        //already late, complete step
        lastTime = current;//can use current because time spent in this function is insignificant compared to minimum usable interval
        return true;
    } else {
        if (-1 == nanosleep(&remaining, nullptr)) {
            //a signal interupted our nap
            return false;
        } else {
            //add time spent sleeping
            lastTime = addTime(current, remaining);
            return true;
        } 
    }
}
