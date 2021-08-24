#pragma once

#include "i_sensor_reader.h"

class LmSensorsReader : ISensorReader{
    public:
        LmSensorsReader();
        int debugListEverything();
        std::vector<std::string> getAll() override;
        int getValue(std::string sensorId) override;
        ~LmSensorsReader();
};