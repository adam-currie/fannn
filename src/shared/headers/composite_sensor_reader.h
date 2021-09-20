#pragma once

#include <vector>
#include "i_sensor_reader.h"

class CompositeSensorReader : public ISensorReader{
    std::vector<ISensorReader> readers;
public:
    CompositeSensorReader() ;
};

