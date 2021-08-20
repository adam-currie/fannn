#include <string>

class ISensorReader {
    public:
        virtual void update() = 0;
        virtual int getValue(std::string sensorId) = 0;
};