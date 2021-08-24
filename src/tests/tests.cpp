#include <catch2/catch_test_macros.hpp>

#include <functional>
#include <iostream>
#include "governor.h"
#include "lm_sensors_reader.h"
#include "sysfs_pwm_writer.h"


using namespace std;

TEST_CASE("parseUserExp_test"){
    int counter = 0;

    function<function<int()>(string)> getSensorDebug = [&counter](string s){
        REQUIRE(s == "debug_temp");
        return [&counter](){ 
            return counter++; 
        };
    };

    function<function<int(int)>(string)> getCurve = [](string s){
        REQUIRE(s == "debug_curve");
        return [](int i){ 
            return i*10; 
        };
    };

    function<int()> f = parseUserExp(
        "input:debug_temp % 3 func:debug_curve",
        getSensorDebug,
        getCurve);
        
    REQUIRE(f() == 0);
    REQUIRE(f() == 10);
    REQUIRE(f() == 20);
    REQUIRE(f() == 0);
}

TEST_CASE("governor_test"){
    int setCounter = 0;
    int a = 0;
    int b = 0;
    int c = 0;

    function<int()> getter = [&](){
        return ++setCounter;
    };
    
    vector<function<void(int)>> setters = {
        [&](int v){a=v;},
        [&](int v){b=v;},
        [&](int v){c=v;}
    };

    function<void()> gov = getGovernor(getter, setters);

    gov();
    REQUIRE(a == 1);
    REQUIRE(b == 1);
    REQUIRE(c == 1);
    gov();
    REQUIRE(a == 2);
    REQUIRE(b == 2);
    REQUIRE(c == 2);
}

TEST_CASE("temp_test"){
    LmSensorsReader().debugListEverything();
}

TEST_CASE("sysfs_pwm_writer_test"){
    SysfsPwmWriter().getAll();
}