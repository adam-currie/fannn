#pragma once

#include "i_sensor_reader.h"
#include <memory>
#include <experimental/propagate_const>

namespace Fannn {

    class LmSensorsReader : public ISensorReader {
        class Impl;
        std::experimental::propagate_const<std::unique_ptr<Impl>> pImpl;
        
        LmSensorsReader();
        LmSensorsReader(LmSensorsReader const&) = delete;
        void operator=(LmSensorsReader const&) = delete;
        LmSensorsReader(LmSensorsReader const&&) = delete;
        void operator=(LmSensorsReader const&&) = delete;

        public:
            
            std::vector<std::string> const getAll() override;
            void scan() override;
            double read(std::string sensorId) override;
            ~LmSensorsReader();

            static LmSensorsReader& instance() {
                static auto instance = LmSensorsReader();
                return instance;
            }
    };

}

extern "C" Fannn::ISensorReader* fannn_reader_plugin_get() {
    return &Fannn::LmSensorsReader::instance();
}
