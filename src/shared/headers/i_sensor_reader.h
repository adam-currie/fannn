#include <string>

class ISensorReader {
    public:
        virtual int getValue(std::string sensorId) = 0;
};