#pragma once

#include "i_device_writer.h"
#include <vector>

namespace Fannn {

    class SysfsPwmWriter : public IDeviceWriter{
        std::vector<std::string> ids;

        SysfsPwmWriter();
        SysfsPwmWriter(SysfsPwmWriter const&) = delete;
        void operator=(SysfsPwmWriter const&) = delete;

        public:
            
            std::vector<std::string> const & getAll() override {
                return ids;
            }

            bool write(std::string deviceId, double value) override;

            void rescan() override {
                //todo
            }

            static SysfsPwmWriter& instance() {
                static auto instance = SysfsPwmWriter();
                return instance;
            }
    };
    
}
