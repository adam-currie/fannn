#pragma once

#include <string>
#include <vector>

namespace Fannn {

    class ISensorReader {
        public:
            virtual std::vector<std::string> getAll() = 0;
            virtual double getValue(std::string sensorId) = 0;
    };

}