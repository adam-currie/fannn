#pragma once

#include "i_sensor_reader.h"
#include <memory>
#include <experimental/propagate_const>

namespace Fannn {

    class LmSensorsReader : ISensorReader{
        class Impl;
        std::experimental::propagate_const<std::unique_ptr<Impl>> pImpl;
        public:
            LmSensorsReader();
            int debugListEverything();
            std::vector<std::string> getAll() override;
            double getValue(std::string sensorId) override;
            ~LmSensorsReader();
    };

}