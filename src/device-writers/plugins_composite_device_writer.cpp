#include "plugins_composite_device_writer.h"

#include <cmath>
#include <filesystem>
#include "containers_util.h"
#include "plugin_dir_loading.h"

using namespace std;
using namespace Fannn;

void PluginsCompositeDeviceWriter::scan() {
    for (auto& p : plugins)
        p.plugin().scan();
    loadErrors.clear();
    Plugins::loadFromDir(
        plugins,
        PLUGINS_DIR,
        [&] (Plugins::PluginLoadError err) -> void {loadErrors.push_back(err);}
    );
}

vector<string> const PluginsCompositeDeviceWriter::getAll() {
    using Fannn::Util::addAll;

    vector<string> v;
    for (IDeviceWriter& p : plugins)
        v>>addAll(p.getAll());
    return v;
}

bool PluginsCompositeDeviceWriter::write(string id, double value) {
    for (auto const & p : plugins)
        if (p.plugin().write(id, value))
            return true;
    return false;
}
