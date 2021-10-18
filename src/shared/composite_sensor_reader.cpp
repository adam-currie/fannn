#include "composite_sensor_reader.h"
#include "lm_sensors_reader.h"

using namespace std;
using namespace Fannn;

CompositeSensorReader::CompositeSensorReader() {
    //todo: dynamically load sensor reader plugins
    readers.push_back(make_unique<LmSensorsReader>());
}

bool CompositeSensorReader::hasSensor(string sensor) const {
    for (auto const & r : readers)
        if (r->hasSensor(sensor))
            return true;
    return false;
}

vector<string> CompositeSensorReader::getAll() const {
    vector<string> v;
    for (auto const & r : readers) {
        vector<string> names = r->getAll();
        v.insert(v.end(), names.begin(), names.end());
    } 
    return v;
}

double CompositeSensorReader::getValue(string sensorId) const {
    for (auto const & r : readers)
        if (r->hasSensor(sensorId))
            return r->getValue(sensorId);
    throwSensorNotFound(sensorId);
    return 0;//never reached(making compiler happy)
}
