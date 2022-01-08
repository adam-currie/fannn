#include <catch2/catch_test_macros.hpp>

#include <functional>
#include <iostream>
#include "governor.h"
#include "i_sensor_reader.h"
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

    c.setDomain(420,421);
    c.setRange(666,666);

    REQUIRE(c.getY(420) == 666);

    uniform_real_distribution<double> r(-1000,1000);
    default_random_engine re;

    vector<Curve::Point> points;
    for (int i=0; i<10000; ++i) {
        Curve::Point p = {r(re), r(re)};
        points.push_back(p);
    }
    c.setPoints(points);
    
    for (const auto & p : c.getPoints()) {
        REQUIRE(p.x >= c.getMinX());
        REQUIRE(p.x <= c.getMaxX());
        REQUIRE(p.y >= c.getMinY());
        REQUIRE(p.y <= c.getMaxY());
    }

    REQUIRE(c.getPoints()[0] == Curve::Point(420, 666));
    REQUIRE(c.getY(420) == 666);
}

TEST_CASE("tokenizer_test"){
    Tokenizer tokenizer(" 1+2 -3+value - 420 ", {' '}, {'+','-'});
    REQUIRE(
            tokenizer.getTokens() ==
            vector<string>({"1","+","2","-","3","+","value","-","420"})
    );
}

TEST_CASE("governor_test"){
    //has fake sensor values that increment each time they are read, starting at 10
    class : public Expression::INamedFuncContext { public:
        string prefix = "sensor";
        mutable vector<double> values = vector<double>(8, 10);
        map<string,Governor> govMap;

        double readSensor(const string& id) const {
            try {
                int i = stoi(id.substr(prefix.size()));
                return (i >= 0 && i < values.size()) ?
                        values[i]++ :
                        numeric_limits<double>::quiet_NaN();
            } catch (...) {
                return numeric_limits<double>::quiet_NaN();
            }
        }
        bool lookupAndExec(const string& id, double & out, string & errMsg) const override {
            out = numeric_limits<double>::quiet_NaN();
            try{
                out = govMap.at(id).constExec(*this);
            }catch(out_of_range){
                out = readSensor(id);
                if (isnan(out))
                    errMsg = "sensor/governor not found";
            }
            return !isnan(out);
        }
        bool lookupAndExec(const string& id, double & out, string & errMsg, double arg) const override {
            static const Curve curve1("curve1",{{0,0},{100,50}});
            out = (id == "curve1") ?
                curve1.getY(arg) :
                numeric_limits<double>::quiet_NaN();
            return !isnan(out);
        }
       bool testLookUpOneArgFunc(const std::string& id, std::string & errMsg) const override {
           bool found = id == "curve1";
           if (!found) errMsg = "i don't have a curve named: " + id;
           return found;
       }
    } context;

    Governor gov1("gov1", "curve1 sensor1");
    context.govMap.insert({gov1.name, gov1});

    Governor gov2(
            "gov2",
            " curve1(gov1     +sensor2 * 2)^2 ^ 3  -2      ");
    context.govMap.insert({gov2.name, gov2});

    REQUIRE(gov1.exec(context) == 5);
    REQUIRE(gov1.exec(context) == 5.5);
    REQUIRE(gov1.exec(context) == 6);
    REQUIRE(gov1.exec(context) == 6.5);

    REQUIRE(fabs(gov2.exec(context) - 1103240374.88) < .01);
}
