#pragma once

#include "dl_plugin.h"
#include <filesystem>
#include "containers_util.h"
#include <memory>
#include <variant>

namespace Fannn::Plugins {

    /**
     * @brief loads any unloaded plugins without unloading old ones
     */
    template<
        template<
            typename TDLPlugin,
            typename = std::allocator<TDLPlugin>
        >
        typename TContainer,
        typename TDLPlugin
    >
    void loadFromDir(TContainer<TDLPlugin>& dlPlugins, const char* dir) {
        using LoadError = typename TDLPlugin::LoadError;

        for (const auto& entry : std::filesystem::directory_iterator(dir)) {
            if (!entry.is_regular_file())
                continue;

            std::variant<TDLPlugin, LoadError> dlPluginOrErr = TContainer<TDLPlugin>::value_type::load(entry.path().c_str());

            if (std::holds_alternative<TDLPlugin>(dlPluginOrErr)) {
                TDLPlugin dlPlugin = std::move(std::get<TDLPlugin>(dlPluginOrErr));
                using Fannn::Util::contains;
                if (dlPlugins>>contains(dlPlugin) == false)
                    dlPlugins.push_back(std::move(dlPlugin));
            } else {
                LoadError err = std::get<LoadError>(dlPluginOrErr);
                if (err.likelyUserError) {
                    //todo: tell them
                }
            }
        }
    }

}
