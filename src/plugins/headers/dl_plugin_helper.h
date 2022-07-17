#pragma once

#include "dl_obj.h"
#include <string>
#include <memory>
#include <experimental/propagate_const>

namespace Fannn::Plugins::Internal {
    std::optional<DlObj> loadPluginDlo(const char * const path, const char * const getterName, bool& looksLikeUserError, std::string& err);
}
