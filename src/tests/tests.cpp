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
#include <random>
#include <math.h>

using namespace std;
using namespace Fannn;

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

    c.setPoints({
        {0,0},
        {10,5}
    });
    REQUIRE(c.getY(9) == 4.5);

    c.setPoints({
        {1,1},
        {2,2}
    });

    c.setDomain(420,420);
    c.setRange(666,666);

    REQUIRE_THROWS_AS( c.getY(419), typeof(out_of_range) );
    REQUIRE_THROWS_AS( c.getY(421), typeof(out_of_range) );
    REQUIRE(c.getY(420) == 666);

    uniform_real_distribution<double> r(-1000,1000);
    default_random_engine re;

    vector<Curve::Point> points;
    for (int i=0; i<10000; ++i) {
        Curve::Point p = {r(re), r(re)};
        points.push_back(p);
    }
    c.setPoints(points);

    REQUIRE(c.getPoints().size() == 1);
    REQUIRE(c.getPoints()[0] == Curve::Point(420, 666));
    REQUIRE(c.getY(420) == 666);
}

TEST_CASE("tokenizer_test"){
    Tokenizer tokenizer(" 1+2 -3+value - 420 ", {' '}, {'+','-'});
    REQUIRE(tokenizer.getTokens() == vector<string>({"1","+","2","-","3","+","value","-","420"}));
}

TEST_CASE("governor_test"){
    auto readCurve = [](string name){
        Curve curve1;
        curve1.setPoints({{0,0},{100,50}});
        if (name == "curve1") {
            return [=](double x){ return curve1.getY(x); };
        }else{
            throw runtime_error("thats not a curve!");
        }
    };

    //fake sensor values that increment each time they are read, starting at 10
    class : public ISensorReader { public:
        string prefix = "sensor";
        mutable vector<double> values = vector<double>(8, 10);
        vector<string> getAll() const override {
            vector<string> all(values.size());
            for(int i=0; i<all.size(); i++) all[i] = i;
            return all;
        }
        double getValue(string id) const override {
            if (!hasSensor(id)) throw runtime_error("that aint no sensor");
            id = id.substr(prefix.size());
            return values[stoi(id)]++;
        }
        bool hasSensor(string id) const override {
            return (id.rfind(prefix, 0) == 0);
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

    Governor gov1;
    gov1.setExpression("curve1 sensor1");
    gov1.readCurve = readCurve;
    gov1.readSensorOrGovernor = readSensorOrGov;
    govMap.insert({"gov1", gov1});

    Governor gov2;
    gov2.setExpression(
        " curve1(gov1     +sensor2 * 2)^2 ^ 3  -2      ");
    gov2.readCurve = readCurve;
    gov2.readSensorOrGovernor = readSensorOrGov;
    govMap.insert({"gov2", gov2});

    REQUIRE(gov1.exec() == 5);
    REQUIRE(gov1.exec() == 5.5);
    REQUIRE(gov1.exec() == 6);
    REQUIRE(gov1.exec() == 6.5);

    REQUIRE(fabs(gov2.exec() - 1103240374.88) < .01);
}
