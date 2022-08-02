#pragma once
#include <string>

namespace Fannn::Plugins {
    struct PluginLoadError {
        std::string path;
        bool likelyUserError;
        std::string msg;
    };
}
