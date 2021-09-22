#pragma once

#include <string>
#include <vector>

namespace Fannn {
        
    class IDeviceWriter {
        public:
            virtual std::vector<std::string> getAll() = 0;
            virtual void setValue(std::string deviceId, int value) = 0;
    };

}