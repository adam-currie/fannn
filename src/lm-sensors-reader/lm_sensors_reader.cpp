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
#include "id_builder.h"

using namespace std;
using namespace Fannn;

struct SensorData {
    sensors_chip_name chip;
    int subFeature;
    SensorData(sensors_chip_name chip, int subFeature) : chip(chip), subFeature(subFeature) {}
}typedef SensorData;

class LmSensorsReader::Impl {
    public:
        map<string, SensorData> sensorMap = map<string, SensorData>();
};

LmSensorsReader::LmSensorsReader() : pImpl{std::make_unique<Impl>()} {
    //lmsensors doc says this shouldnt be called more than once, 
    //i have no idea why but if this is ever not a singleton we should find out
    sensors_init(nullptr);

    sensors_chip_name const *chip;
    int nextChip = 0;
    while ((chip = sensors_get_detected_chips(0, &nextChip)) != 0) {
        sensors_feature const *feature;
        int nextFeature = 0;
        while ((feature = sensors_get_features(chip, &nextFeature)) != 0) {
            string featureLabel = sensors_get_label(chip, feature);
            SensorData data(*chip, feature->first_subfeature);
            IdBuilder idBuilder;
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

double LmSensorsReader::read(string sensorId) {
    auto iter = pImpl->sensorMap.find(sensorId);

    if (iter == pImpl->sensorMap.end())
        numeric_limits<double>::quiet_NaN();

    SensorData& sensor = iter->second;

    double value;
    if (sensors_get_value(&sensor.chip, sensor.subFeature, &value) < 0) {
        return numeric_limits<double>::quiet_NaN();//todo: remove from sensor map??
    }

    return value;
}

vector<string> const LmSensorsReader::getAll() {
    //todo: cache
    vector<string> keys(pImpl->sensorMap.size());
    int i = 0;
    for (auto pair : pImpl->sensorMap) {
        keys[i++] = pair.first;
    }
    return keys;
}

void LmSensorsReader::scan() {
    //todo
}

LmSensorsReader::~LmSensorsReader() {
    sensors_cleanup();
}
