#pragma once

#include <vector>
#include "i_device_writer.h"

namespace Fannn {

    class CompositeDeviceWriter : public IDeviceWriter{
        std::vector<IDeviceWriter> writers;
    public:
        CompositeDeviceWriter() ;
    };

}