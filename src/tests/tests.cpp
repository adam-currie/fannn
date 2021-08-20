#include <catch2/catch_test_macros.hpp>

#include <functional>
#include <iostream>
#include "governor.h"
#include "reader.h"


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

TEST_CASE("temp_test"){
    Reader().debugListEverything();
}