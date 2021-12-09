#include "composite_sensor_reader.h"
#include "lm_sensors_reader.h"
#include <cmath>

using namespace std;
using namespace Fannn;

CompositeSensorReader::CompositeSensorReader() {
    //todo: dynamically load sensor reader plugins
    readers.push_back(&LmSensorsReader::instance());
}

void CompositeSensorReader::rescan() {
    //todo
}

vector<string> const CompositeSensorReader::getAll() {
    vector<string> v;
    for (auto const & r : readers) {
        vector<string> names = r->getAll();
        v.insert(v.end(), names.begin(), names.end());
    } 
    return v;
}

double CompositeSensorReader::read(string sensorId) {
    for (auto const & r : readers) {
        double value = r->read(sensorId);
        if (!isnan(value))
            return value;
    }
    return numeric_limits<double>::quiet_NaN();
}
