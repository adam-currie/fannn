#include <string>
#include <vector>

class ISensorReader {
    public:
        virtual std::vector<std::string> getAll() = 0;
        virtual int getValue(std::string sensorId) = 0;
};