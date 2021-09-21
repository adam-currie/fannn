#include <catch2/catch_test_macros.hpp>

#include <functional>
#include <iostream>
#include "governor.h"
#include "lm_sensors_reader.h"
#include "sysfs_pwm_writer.h"
#include "curve.h"
#include "profile.h"
#include "profile_engine.h"
#include "ranges"
#include "tokenizer.h"
#include <math.h>

using namespace std;

TEST_CASE("sensors_get_all_test"){
    for(string str : LmSensorsReader().getAll()){
        cout << str << endl;
    }
}

TEST_CASE("sysfs_pwm_writer_test"){
    auto writer = SysfsPwmWriter();
    auto devices = writer.getAll();
    writer.setValue("/sys/devices/platform/nct6687.2592/hwmon/hwmon4/pwm1", 50);
}

TEST_CASE("curve_gety_test"){
    Curve c;

    REQUIRE_THROWS_AS( c.setDomain(101, 100), typeof(out_of_range) );
    REQUIRE_THROWS_AS( c.setRange(101, 100), typeof(out_of_range) );

    c.points = map<double,double>({
        {0,0},
        {10,5}
    });
    REQUIRE(c.getY(9) == 4.5);

    c.points = map<double,double>({
        {1,1},
        {2,2}
    });

    c.setDomain(420,420);
    c.setRange(666,666);
    REQUIRE_THROWS_AS( c.getY(419), typeof(out_of_range) );
    REQUIRE_THROWS_AS( c.getY(421), typeof(out_of_range) );
    REQUIRE(c.getY(420) == 666);
}

TEST_CASE("tokenizer_test"){
    Tokenizer tokenizer(" 1+2 -3+value - 420 ", {' '}, {'+','-'});
    REQUIRE(tokenizer.getTokens() == vector<string>({"1","+","2","-","3","+","value","-","420"}));
}

TEST_CASE("governor_test"){
    auto readCurve = [](string name){
        Curve curve1;
        curve1.points = {{0,0},{100,50}};
        if (name == "curve1") {
            return [=](double x){ return curve1.getY(x); };
        }else{
            throw runtime_error("thats not a curve!");
        }
    };

    //fake sensor values that increment each time they are read, starting at 10
    class : public ISensorReader { public:
        string prefix = "sensor";
        vector<double> values = vector<double>(8, 10);
        vector<string> getAll(){
            vector<string> all(values.size());
            for(int i=0; i<all.size(); i++) all[i] = i;
            return all;
        }
        double getValue(string id){
            if (id.rfind(prefix, 0) != 0) throw runtime_error("that aint no sensor");
            id = id.substr(prefix.size());
            return values[stoi(id)]++;
        }
    } reader;

    map<string,Governor> govMap;

    auto readSensorOrGov = [&](string id){ 
        try{
            return govMap.at(id).exec();
        }catch(out_of_range){
            return reader.getValue(id); 
        }
    };

    Governor gov1(
        "curve1 sensor1",
        [](string s){return true;},
        [](string s){return true;});
    gov1.readCurve = readCurve;
    gov1.readSensorOrGovernor = readSensorOrGov;
    govMap.insert({"gov1", gov1});

    Governor gov2(
        " curve1(gov1     +sensor2 * 2)^2 ^ 3  -2      ",
        [](string s){return true;},
        [](string s){return true;});
    gov2.readCurve = readCurve;
    gov2.readSensorOrGovernor = readSensorOrGov;
    govMap.insert({"gov2", gov2});

    REQUIRE(gov1.exec() == 5);
    REQUIRE(gov1.exec() == 5.5);
    REQUIRE(gov1.exec() == 6);
    REQUIRE(gov1.exec() == 6.5);

    REQUIRE(fabs(gov2.exec() - 1103240374.88) < .01);
}