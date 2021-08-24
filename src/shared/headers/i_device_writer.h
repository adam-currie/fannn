#include <string>
#include <vector>

class IDeviceWriter {
    public:
        virtual std::vector<std::string> getAll() = 0;
        virtual void setValue(std::string deviceId) = 0;
};