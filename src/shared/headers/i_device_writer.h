#pragma once

#include <string>
#include <vector>
#include <stdexcept>

namespace Fannn {
        
    class IDeviceWriter {
        public:
            virtual std::vector<std::string> const & getAll() const = 0;
            virtual void setValue(std::string deviceId, double value) = 0;
        protected:
            void throwDeviceNotFound(std::string writerId) const {
                throw std::out_of_range("'" + writerId + "' is not a valid device");
            }
    };

}
