#pragma once

#include <vector>
#include "i_sensor_reader.h"

namespace Fannn {

    class CompositeSensorReader : public ISensorReader{
        std::vector<ISensorReader> readers;
    public:
        CompositeSensorReader() ;
    };

}