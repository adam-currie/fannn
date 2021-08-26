#include <time.h>
#include "governor.h"
#include "sysfs_pwm_writer.h"
#include "lm_sensors_reader.h"
#include <sensors/error.h>
#include <stdexcept>
#include <iostream>

using namespace std;

void loadConfig();
void sleepMs(int milliSeconds);

//todo: should be using an abstraction layer with prefixes on the device id str for different underlying writers
SysfsPwmWriter writer = SysfsPwmWriter();
//todo: should be using an abstraction layer
LmSensorsReader reader = LmSensorsReader();
int updateIntervalMs = 2;
vector<function<void()>> governors;

int main(){
    loadConfig();
    int i = 0;//debug
    while(1){
        for(auto g : governors) g();
        std::cout << i++ << std::endl;//debug
        sleepMs(updateIntervalMs);
    }

    return 0;
}

void loadConfig(){
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

    auto junctionTempTimes2 = parseUserExp(
        "input:amdgpu/2/0/11520/junction/temp2_input * 2",
        getSensor,
        getCurve);

    auto pwm1Setter = [](double v){
        writer.setValue("/sys/devices/platform/nct6687.2592/hwmon/hwmon4/pwm1", v);
    };

    governors.push_back(getGovernor(junctionTempTimes2, {pwm1Setter}));
}

void sleepMs(int milliseconds){
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
}