#include "lm_sensors_reader.h"
#include <string.h>
#include <sensors/sensors.h>
#include <sensors/error.h>
#include <iostream>
#include <sstream>
#include <atomic>
#include <map>

using namespace std;

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

class deliminatedStrBuilder{
    ostringstream out;
    char delim, escape;
    bool hasData = false;
    public:
        deliminatedStrBuilder(char delim, char escape) : delim(delim), escape(escape) {}
        deliminatedStrBuilder& operator<<(string s){
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
        string get(){ return out.str(); }
};

//debug: old
// /**
//  * @brief splits the sensor id into its 2 parts and nullifies the deliminator, nothing is new-ed
//  * @return int 0 if successful
//  */
// int splitSensorIdStr(char *idStr, const char **chipStr, const char **subFeatureNumStr){
//     *chipStr = idStr;
//     bool escaping = false;

//debug: old
//     for(char *c = idStr; *c != '\0'; c++){
//         if(escaping){
//             escaping = false;
//         }else{
//             if(*c == IDSTR_DELIM){
//                 *c = '\0';
//                 *subFeatureNumStr = c+1;
//                 return 0;
//             }
//         }
//     }
//     return 1;
// }

// //negative on failure
// int cStrToPositiveInt(const char *str){
//     long l = strtol(str, nullptr, 10);
//     return (l >= 0 && l <= INT_MAX)? l : -1;
// }

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
            sensors_subfeature const *subFeature;
            int nextSubFeature = 0;
            string featureLabel = sensors_get_label(chip, feature);
            while ((subFeature = sensors_get_all_subfeatures(chip, feature, &nextSubFeature)) != 0) {
                if(subFeature->flags & SENSORS_MODE_R){
                    SensorData data(*chip, nextSubFeature-1);
                    deliminatedStrBuilder idBuilder(IDSTR_DELIM, IDSTR_ESCAPE);
                    idBuilder << chip->prefix << to_string(chip->bus.type) << to_string(chip->bus.nr) << to_string(chip->addr) << featureLabel << subFeature->name;
                    pImpl->sensorMap.insert({idBuilder.get(), data});//todo: deal with dups
                }
            }
        }
    }
}

double LmSensorsReader::getValue(string sensorId){
    SensorData sensor = pImpl->sensorMap.at(sensorId);

    double value;
    if(sensors_get_value(&sensor.chip, sensor.subFeature, &value)){
        throw logic_error("failed to read value of sensor: " + sensorId);//todo: better
    }

    return value;
}

vector<string> LmSensorsReader::getAll(){
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