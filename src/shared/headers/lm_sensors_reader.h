#pragma once

#include "i_sensor_reader.h"
#include <memory>
#include <experimental/propagate_const>

namespace Fannn {

    class LmSensorsReader : public ISensorReader{
        class Impl;
        std::experimental::propagate_const<std::unique_ptr<Impl>> pImpl;
        public:
            LmSensorsReader();
            int debugListEverything();
            bool hasSensor(std::string sensorId) const override;
            std::vector<std::string> getAll() const override;
            double getValue(std::string sensorId) const override;
            ~LmSensorsReader();
    };

}