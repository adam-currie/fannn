#pragma once

#include "i_device_writer.h"

namespace Fannn {

    class SysfsPwmWriter : IDeviceWriter{
        public:
            SysfsPwmWriter();
            std::vector<std::string> getAll() override;
            void setValue(std::string deviceId, int value) override;
    };
    
}