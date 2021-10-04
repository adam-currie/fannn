#pragma once

#include <vector>
#include <memory>
#include "i_sensor_reader.h"

namespace Fannn {

    class CompositeSensorReader : public ISensorReader {
        std::vector<std::unique_ptr<ISensorReader>> readers;
        public:
            CompositeSensorReader();
            bool hasSensor(std::string sensor) const override;
            std::vector<std::string> getAll() const override;
            double getValue(std::string sensorId) const override;
    };

}