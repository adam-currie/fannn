#pragma once

#include <vector>
#include <optional>
#include "i_sensor_reader.h"
#include "dl_plugin.h"

namespace Fannn {

    class PluginsCompositeSensorReader : public ISensorReader {
        static constexpr char PLUGINS_DIR[] = "plugins/";
        static constexpr char PLUGIN_GETTER_NAME[] = "fannn_reader_plugin_get";

        std::vector<Plugins::DlPlugin<PLUGIN_GETTER_NAME, ISensorReader>> plugins;

        PluginsCompositeSensorReader();
        PluginsCompositeSensorReader(PluginsCompositeSensorReader const &) = delete;
        PluginsCompositeSensorReader(PluginsCompositeSensorReader const &&) = delete;
        void operator=(PluginsCompositeSensorReader const &) = delete;
        void operator=(PluginsCompositeSensorReader const &&) = delete;

        public:
            std::vector<std::string> const getAll() override;
            void rescan() override;
            double read(std::string sensorId) override;

            static PluginsCompositeSensorReader& instance() {
                static auto instance = PluginsCompositeSensorReader();
                return instance;
            }
    };

}
