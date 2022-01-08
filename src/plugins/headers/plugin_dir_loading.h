#pragma once

#include "dl_plugin.h"
#include <filesystem>
#include "containers_util.h"

namespace Fannn::Plugins {

    /**
     * @brief loads any unloaded plugins without unloading old ones
     */
    template<typename TContainer>
    void loadFromDir(TContainer& plugins, const char* dir) {
        for (const auto& entry : std::filesystem::directory_iterator(dir)) {
            if (!entry.is_regular_file())
                continue;

            auto plugin = TContainer::value_type::load(entry.path().c_str());

            using Fannn::Util::contains;

            if (plugin && plugins>>contains(*plugin) == false)
                plugins.push_back(std::move(*plugin));
        }
    }

}
