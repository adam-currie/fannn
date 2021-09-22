#include <time.h>
#include "governor.h"
#include "sysfs_pwm_writer.h"
#include "lm_sensors_reader.h"
#include "user_config.h"
#include <sensors/error.h>
#include <iostream>
#include <cmath>

using namespace std;
using namespace Fannn;

/**
 * @brief waits just long enough to make a step take as long as it should, taking into account the time already spent on it
 */
class MinIntervalStepper{
    timespec lastTime = {0,0};
    timespec interval;
    static const long NS_IN_A_SEC = 1000000000;
    void getTime(timespec *out){
        if(clock_gettime(CLOCK_MONOTONIC, out))
            throw runtime_error("clock_gettime() failed:" + errno);
    }
    public: 
        /**
         * @brief Construct a new Min Interval Stepper object
         * @param minIntervalMs default is zero, resulting in no waiting between steps.
         */
        MinIntervalStepper(int minIntervalMs = 0) : 
            interval{minIntervalMs/1000, (minIntervalMs % 1000) * (NS_IN_A_SEC/1000)} {}
        void step(){
            timespec current, elapsed, remaining;
            getTime(&current);

            elapsed.tv_sec = current.tv_sec - lastTime.tv_sec;
            elapsed.tv_nsec = current.tv_nsec - lastTime.tv_nsec;

            remaining.tv_nsec = interval.tv_nsec-elapsed.tv_nsec;
            remaining.tv_sec = interval.tv_sec-elapsed.tv_sec;
            //carry up nsec wrap arounds
            remaining.tv_sec += remaining.tv_nsec / NS_IN_A_SEC;
            remaining.tv_nsec %= NS_IN_A_SEC;

            /*if seconds remaining was 0 but ns remaining was negative 
              it would have rolled over and made seconds negative instead, 
              so as long as seconds isn't negative we have waiting to do */
            if(remaining.tv_sec >= 0){
                nanosleep(&remaining, nullptr);
                //add time spent sleeping
                lastTime.tv_sec = current.tv_sec + remaining.tv_sec;
                lastTime.tv_nsec = current.tv_nsec + remaining.tv_nsec;
                //carry up nsec wrap arounds
                lastTime.tv_sec += lastTime.tv_nsec / NS_IN_A_SEC;
                lastTime.tv_nsec %= NS_IN_A_SEC;
            }else{
                //time spent in this function is insignificant compared to minimum usable interval
                lastTime = current;
            }
        }
};

void fakeConfigLoad();

//todo: should be using an abstraction layer with prefixes on the device id str for different underlying writers
SysfsPwmWriter writer = SysfsPwmWriter();
//todo: should be using an abstraction layer
LmSensorsReader reader = LmSensorsReader();
vector<function<void()>> governors;
MinIntervalStepper stepper;

int main(){
    UserConfig config("debug_filename");
    config.load();

    int i = 0;//debug
    while(1){
        stepper.step();
        for(auto g : governors) 
            g();
        std::cout << i++ << std::endl;//debug
        if(config.checkForExternalChanges())
            config.load();
        fakeConfigLoad();//debug
    }
}

//debug
void fakeConfigLoad(){
    function<function<double()>(string)> getSensor = [](string s){
        return [s](){ 
            return reader.getValue(s); 
        };
    };

    //todo
    //debug: fake loading

    function<function<double(double)>(string)> getCurve = [](string s){
        return [](double i){ 
            return i;//todo
        };
    };

    //todo:
}
