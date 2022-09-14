#pragma once

#include <vector>
#include <optional>
#include "i_device_writer.h"
#include "dl_plugin.h"

namespace Fannn {

    class PluginsCompositeDeviceWriter : public IDeviceWriter {
        static constexpr char PLUGINS_DIR[] = "plugins/";
        static constexpr char PLUGIN_GETTER_NAME[] = "fannn_writer_plugin_get";

        std::vector<Plugins::DlPlugin<PLUGIN_GETTER_NAME, IDeviceWriter>> plugins = {};
        std::vector<Plugins::PluginLoadError> loadErrors = {};

        PluginsCompositeDeviceWriter() {}
        PluginsCompositeDeviceWriter(PluginsCompositeDeviceWriter const &) = delete;
        PluginsCompositeDeviceWriter(PluginsCompositeDeviceWriter const &&) = delete;
        void operator=(PluginsCompositeDeviceWriter const &) = delete;
        void operator=(PluginsCompositeDeviceWriter const &&) = delete;

        public:
            std::vector<std::string> const getAll() override;
            void scan() override;
            bool write(std::string id, double value) override;

            std::vector<Plugins::PluginLoadError> getPluginLoadErrors() const {
                return loadErrors;
            }

            static PluginsCompositeDeviceWriter& instance() {
                static auto instance = PluginsCompositeDeviceWriter();
                return instance;
            }
    };

}
