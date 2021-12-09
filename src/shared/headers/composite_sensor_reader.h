#pragma once

#include <vector>
#include <memory>
#include "i_sensor_reader.h"

namespace Fannn {

    class CompositeSensorReader : public ISensorReader {
        std::vector<ISensorReader*> readers;

        CompositeSensorReader();
        CompositeSensorReader(CompositeSensorReader const &) = delete;
        CompositeSensorReader(CompositeSensorReader const &&) = delete;
        void operator=(CompositeSensorReader const &) = delete;
        void operator=(CompositeSensorReader const &&) = delete;

        public:
            std::vector<std::string> const getAll() override;
            void rescan() override;
            double read(std::string sensorId) override;

            static CompositeSensorReader& instance() {
                static auto instance = CompositeSensorReader();
                return instance;
            }
    };

}
