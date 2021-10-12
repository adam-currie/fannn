#include "lm_sensors_reader.h"
#include <string.h>
#include <sensors/sensors.h>
#include <sensors/error.h>
#include <iostream>
#include <sstream>
#include <atomic>
#include <map>
#include <iomanip>
#include <cmath>

using namespace std;
using namespace Fannn;

const char IDSTR_DELIM = '/';
const char IDSTR_ESCAPE = '\\';

static atomic<LmSensorsReader*> instance(nullptr);

struct SensorData {
    sensors_chip_name chip;
    int subFeature;
    SensorData(sensors_chip_name chip, int subFeature) : chip(chip), subFeature(subFeature) {}
}typedef SensorData;

class LmSensorsReader::Impl {
    public:
        map<string, SensorData> sensorMap = map<string, SensorData>();
};

class deliminatedStrBuilder {
    ostringstream out;
    char delim, escape;
    bool hasData = false;
    public:
        deliminatedStrBuilder(char delim, char escape) : delim(delim), escape(escape) {}
        deliminatedStrBuilder& operator<<(string s) {
            if(s.empty()) 
                return *this;

            if(hasData) {
                //add delim between strings
                out << delim;
            }else{
                //no delim needed before first string, next time though
                hasData = true;
            }

            for(char c : s){
                if(c == escape || c == delim)
                    out << escape;
                out << c;
            }

            return *this;
        }
        deliminatedStrBuilder& operator<<(int n) {
            static const char digits[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};

            int digitCount = (31 - __builtin_clz(n))/4 + 1;
            string s(digitCount, 0);

            for(int i = digitCount-1; i>=0; i--) {
                s[i] = digits[n%16];
                n /= 16;
            }

            return operator<<(s);
        }
        string get() { return out.str(); }
};

LmSensorsReader::LmSensorsReader() : pImpl{std::make_unique<Impl>()} {
    LmSensorsReader *np = nullptr;
    if(!atomic_compare_exchange_strong(&instance, &np, this)){
        throw logic_error(
            "not implemented: I have no idea why libsensors"
            "doesn't want us calling sensors_init more than once,"
            "but i don't feel like finding out."
            "Probably we can just ignore this or work around it.");
    };

    sensors_init(nullptr);

    sensors_chip_name const *chip;
    int nextChip = 0;
    while ((chip = sensors_get_detected_chips(0, &nextChip)) != 0) {
        sensors_feature const *feature;
        int nextFeature = 0;
        while ((feature = sensors_get_features(chip, &nextFeature)) != 0) {
            string featureLabel = sensors_get_label(chip, feature);
            SensorData data(*chip, feature->first_subfeature);
            deliminatedStrBuilder idBuilder(IDSTR_DELIM, IDSTR_ESCAPE);
            idBuilder << chip->prefix << chip->addr << featureLabel;
            pImpl->sensorMap.insert({idBuilder.get(), data});//todo: deal with dups
            //debug
//            sensors_subfeature const *subFeature;
//            int nextSubFeature = 0;
//            while ((subFeature = sensors_get_all_subfeatures(chip, feature, &nextSubFeature)) != 0) {
//                if(subFeature->flags & SENSORS_MODE_R){
//                    SensorData data(*chip, nextSubFeature-1);
//                    deliminatedStrBuilder idBuilder(IDSTR_DELIM, IDSTR_ESCAPE);
//                    idBuilder << chip->prefix << chip->addr << featureLabel << subFeature->name;
//                    pImpl->sensorMap.insert({idBuilder.get(), data});//todo: deal with dups
//                }
//            }
        }
    }
}

bool LmSensorsReader::hasSensor(string sensorId) const {
    return pImpl->sensorMap.contains(sensorId);
}

double LmSensorsReader::getValue(string sensorId) const {
    if (!hasSensor(sensorId)) throwSensorNotFound(sensorId);

    SensorData sensor = pImpl->sensorMap.at(sensorId);

    double value;
    if(sensors_get_value(&sensor.chip, sensor.subFeature, &value)){
        throw logic_error("failed to read value of sensor: " + sensorId);//todo: better
    }

    return value;
}

vector<string> LmSensorsReader::getAll() const {
    vector<string> keys(pImpl->sensorMap.size());
    int i=0;
    for(auto pair : pImpl->sensorMap){
        keys[i++] = pair.first;
    }
    return keys;
}

LmSensorsReader::~LmSensorsReader() {
    sensors_cleanup();
    instance.store(nullptr);
}
