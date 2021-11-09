#pragma once

#include "i_device_writer.h"
#include <vector>

namespace Fannn {

    class SysfsPwmWriter : public IDeviceWriter{
        std::vector<std::string> ids;
        public:
            SysfsPwmWriter();
            std::vector<std::string> const & getAll() const override {
                return ids;
            }
            void setValue(std::string deviceId, double value) override;
    };
    
}
