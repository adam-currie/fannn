#include "composite_device_writer.h"
#include "sysfs_pwm_writer.h"

using namespace std;
using namespace Fannn;

CompositeDeviceWriter::CompositeDeviceWriter() {
    //todo: dynamically load sensor reader plugins
    writers.push_back(make_unique<SysfsPwmWriter>());

    for (auto const & r : writers) {
        vector<string> const & names = r->getAll();
        //todo: prefix duplicates from different writers
        allIDs.insert(allIDs.end(), names.begin(), names.end());
    }
}


vector<string> const & CompositeDeviceWriter::getAll() const {
    return allIDs;
}

void CompositeDeviceWriter::setValue(string deviceId, double value) {
    //todo
}
