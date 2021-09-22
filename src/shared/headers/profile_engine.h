#pragma once

#include "i_device_writer.h"
#include "i_sensor_reader.h"
#include "profile.h"
#include <memory>

namespace Fannn {
        
    class ProfileEngine {
        private:
            const Profile profile;
            const std::shared_ptr<const IDeviceWriter> deviceWriter;
            const std::shared_ptr<const ISensorReader> sensorReader;
        public:
            ProfileEngine(
                Profile profile, 
                const std::shared_ptr<const IDeviceWriter> deviceWriter, 
                const std::shared_ptr<const ISensorReader> sensorReader);
            void runOnce();
    };

}