#pragma once

#include <vector>
#include <memory>
#include "i_device_writer.h"

namespace Fannn {

    class CompositeDeviceWriter : public IDeviceWriter {
        std::vector<std::unique_ptr<IDeviceWriter>> writers;
        std::vector<std::string> allIDs;

        CompositeDeviceWriter();
        CompositeDeviceWriter(CompositeDeviceWriter const&);
        void operator=(CompositeDeviceWriter const&);

        public:
            std::vector<std::string> const & getAll() const override;
            void setValue(std::string controllerId, double value) override;

            static CompositeDeviceWriter& instance() {
                static CompositeDeviceWriter instance;
                return instance;
            }
    };

}
