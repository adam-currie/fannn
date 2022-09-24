#pragma once
#include <string>

namespace Fannn::Plugins {
    struct PluginLoadError {
        PluginLoadError(std::string path, bool likelyUserError = false, std::string msg = "")
            : path(path), likelyUserError(likelyUserError), msg(msg) {}
        std::string path;
        bool likelyUserError;
        std::string msg;
    };
}
