#include "plugins_composite_sensor_reader.h"

#include <cmath>
#include <dlfcn.h>
#include <filesystem>
#include "containers_util.h"
#include "plugin_dir_loading.h"

using namespace std;
using namespace Fannn;

PluginsCompositeSensorReader::PluginsCompositeSensorReader() {
    Plugins::loadFromDir(plugins, PLUGINS_DIR);
}

void PluginsCompositeSensorReader::rescan() {
    for (auto& p : plugins)
        p.plugin().rescan();
    Plugins::loadFromDir(plugins, PLUGINS_DIR);
}

vector<string> const PluginsCompositeSensorReader::getAll() {
    using Fannn::Util::addAll;

    vector<string> v;
    for (ISensorReader& p : plugins)
        v>>addAll(p.getAll());
    return v;
}

double PluginsCompositeSensorReader::read(string sensorId) {
    for (auto const & p : plugins) {
        double value = p.plugin().read(sensorId);
        if (!isnan(value))
            return value;
    }
    return numeric_limits<double>::quiet_NaN();
}
