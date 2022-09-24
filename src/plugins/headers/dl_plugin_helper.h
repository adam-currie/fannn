#pragma once

#include "dl_obj.h"
#include "plugin_load_error.h"
#include <variant>
#include <string>
#include <memory>
#include <experimental/propagate_const>

namespace Fannn::Plugins::Internal {
    std::variant<DlObj, PluginLoadError> loadPluginDlo(const char * const path, const char * const getterName);
}
