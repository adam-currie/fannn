#include <vector>
#include "i_device_writer.h"

class CompositeDeviceWriter : public IDeviceWriter{
    std::vector<IDeviceWriter> writers;
public:
    CompositeDeviceWriter() ;
};

