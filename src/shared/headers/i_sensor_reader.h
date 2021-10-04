#pragma once

#include <string>
#include <vector>
#include <stdexcept>

namespace Fannn {

    class ISensorReader {
        public:
            virtual bool hasSensor(std::string sensor) const = 0;
            virtual std::vector<std::string> getAll() const = 0;
            virtual double getValue(std::string sensorId) const = 0;
        protected:
            void throwSensorNotFound(std::string sensorId) const {
                throw std::out_of_range("'" + sensorId + "' is not a valid sensor");
            }
    };

}