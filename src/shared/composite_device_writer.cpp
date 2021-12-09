#include "composite_device_writer.h"
#include "sysfs_pwm_writer.h"

using namespace std;
using namespace Fannn;

CompositeDeviceWriter::CompositeDeviceWriter() {
    //todo: dynamically load sensor reader plugins
    writers.push_back(&SysfsPwmWriter::instance());

    for (auto writer : writers) {
        vector<string> const & names = writer->getAll();
        //todo: prefix duplicates from different writers
        allIDs.insert(allIDs.end(), names.begin(), names.end());
    }
}


vector<string> const & CompositeDeviceWriter::getAll() {
    //todo: need to replace this because some writers might change their set of devices dynamically, all calling code needs to be updated too!
    return allIDs;
}

bool CompositeDeviceWriter::write(string deviceId, double value) {
    for (auto writer : writers)
        if (writer->write(deviceId, value))
            return true;
    return false;
}
