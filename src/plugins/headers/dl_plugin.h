#pragma once

#include <variant>
#include <string>
#include "dl_plugin_helper.h"
#include "dl_obj.h"
#include "plugin_load_error.h"

namespace Fannn::Plugins {

    template<const char * const PLUGIN_GETTER_NAME, typename TPlugin>
    class DlPlugin {
        Fannn::Plugins::Internal::DlObj dlo;
        TPlugin* _plugin;

        DlPlugin(Fannn::Plugins::Internal::DlObj&& dlo, TPlugin* plugin)
            : dlo(std::move(dlo)), _plugin(plugin) {}

        public:
            static std::variant<DlPlugin, PluginLoadError> load(const char * path) {
                using Fannn::Plugins::Internal::DlObj;

                std::variant<DlObj, PluginLoadError> tempDloOrErr = Internal::loadPluginDlo(path, PLUGIN_GETTER_NAME);
                if (std::holds_alternative<PluginLoadError>(tempDloOrErr))
                    return std::get<PluginLoadError>(tempDloOrErr);

                DlObj tempDlo = std::move(std::get<DlObj>(tempDloOrErr));

                auto getter = (TPlugin*(*)()) tempDlo.getSymbol(PLUGIN_GETTER_NAME);

                TPlugin* tempPlugin;
                try {
                    tempPlugin = getter();
                }  catch (...) {
                    //todo: wrap, rethrow, document, check in calling code
                    throw;
                }

                return DlPlugin<PLUGIN_GETTER_NAME, TPlugin>(
                        std::move(tempDlo),
                        tempPlugin
                );
            }

            bool operator==(const DlPlugin &other) const {
                return dlo == other.dlo;
            }

            TPlugin& plugin()           { return *_plugin; }
            TPlugin& plugin()   const   { return *_plugin; }
            operator TPlugin&() const   { return *_plugin; }
            operator TPlugin&()         { return *_plugin; }

            DlPlugin(DlPlugin&&) = default;
            DlPlugin& operator=(DlPlugin&&) = default;

            DlPlugin(TPlugin const& other) = delete;
            TPlugin &operator=(TPlugin const& other) = delete;

            ~DlPlugin() = default;
    };

}
