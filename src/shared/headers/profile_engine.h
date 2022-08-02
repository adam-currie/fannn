#pragma once

#include "i_device_writer.h"
#include "profile.h"
#include "profile_governor_exec_context.h"
#include "i_sensor_reader.h"
#include <memory>

namespace Fannn {
        
    class ProfileEngine {
        private:
            typedef struct ControllerWithIndexedGov {
                std::string id;
                size_t govIndex;
            } ControllerWithIndexedGov;

            Profile profile;
            ProfileGovernorExecContext context;
            std::vector<ControllerWithIndexedGov> controllers;
            IDeviceWriter& deviceWriter;
            bool pendingRescan = false;
        public:
            ProfileEngine (Profile profile, IDeviceWriter& deviceWriter, ISensorReader& sensorReader);

            //if we allow any copying we need to make sure we aren't using the old pointer to the profile from the other engine!
            ProfileEngine(ProfileEngine &) = delete;

            void runOnce();
    };

}
