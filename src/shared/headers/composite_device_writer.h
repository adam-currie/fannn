#pragma once

#include <vector>
#include <memory>
#include "i_device_writer.h"

namespace Fannn {

    class CompositeDeviceWriter : public IDeviceWriter {
        std::vector<IDeviceWriter*> writers;
        std::vector<std::string> allIDs;

        CompositeDeviceWriter();
        
        CompositeDeviceWriter(CompositeDeviceWriter const &) = delete;
        CompositeDeviceWriter(CompositeDeviceWriter const &&) = delete;
        void operator=(CompositeDeviceWriter const &) = delete;
        void operator=(CompositeDeviceWriter const &&) = delete;

        public:
            std::vector<std::string> const & getAll() override;
            bool write(std::string controllerId, double value) override;
            void rescan() override {}//todo

            static CompositeDeviceWriter& instance() {
                static auto instance = CompositeDeviceWriter();
                return instance;
            }
    };

}
