#pragma once

#include <vector>
#include <memory>
#include "i_sensor_reader.h"

namespace Fannn {

    class CompositeSensorReader : public ISensorReader {
        std::vector<std::unique_ptr<ISensorReader>> readers;

        CompositeSensorReader();
        CompositeSensorReader(CompositeSensorReader const&);
        void operator=(CompositeSensorReader const&);

        public:
            bool hasSensor(std::string sensor) const override;
            std::vector<std::string> getAll() const override;
            double getValue(std::string sensorId) const override;

            static CompositeSensorReader& instance() {
                static CompositeSensorReader instance;
                return instance;
            }
    };

}
