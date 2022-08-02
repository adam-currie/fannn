#include "plugins_composite_sensor_reader.h"

#include <cmath>
#include <filesystem>
#include "containers_util.h"
#include "plugin_dir_loading.h"

using namespace std;
using namespace Fannn;

void PluginsCompositeSensorReader::rescan() {
    for (auto& p : plugins)
        p.plugin().rescan();
    loadErrors.clear();
    Plugins::loadFromDir(
        plugins,
        PLUGINS_DIR,
        [&] (Plugins::PluginLoadError err) -> void {loadErrors.push_back(err);}
    );
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
