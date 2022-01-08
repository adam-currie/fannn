#pragma once

#include <optional>

namespace Fannn::Plugins {

    namespace Private {
        #include <dlfcn.h>
    }

    template<const char* PLUGIN_GETTER_NAME, typename TPlugin>
    class DlPlugin {
        void* handle;
        TPlugin* _plugin;

        DlPlugin() = default;//use load function, dont let calling code make these

        public:
            static std::optional<DlPlugin> load(const char* path) {
                DlPlugin dlPlugin;

                dlPlugin.handle = Private::dlopen(path, RTLD_NOW);
                if (!dlPlugin.handle)
                    return std::nullopt;

                // don't need to check dlerror because we are checking for null, and we don't want null even if it's not an error
                auto getter = (TPlugin*(*)()) Private::dlsym(dlPlugin.handle, PLUGIN_GETTER_NAME);

                if (!getter)
                    return std::nullopt;

                /*
                 * nothing is an error yet as it's okay to have things other than
                 * plugins in this dir, but everything after here is an error in the plugin
                 */

                try {
                    dlPlugin._plugin = getter();
                }  catch (...) {
                    //todo: wrap, rethrow, document, check in calling code
                    throw;
                }

                //i STILL don't trust it! let me at least check for null
                if (dlPlugin._plugin) {
                    return std::optional(std::move(dlPlugin));
                } else {
                    //todo: better exception
                    throw;
                }
            }

            bool operator==(const DlPlugin &other) const {
                return handle == other.handle;
            }

            TPlugin& plugin()           { return *_plugin; }
            TPlugin& plugin()   const   { return *_plugin; }
            operator TPlugin&() const   { return *_plugin; }
            operator TPlugin&()         { return *_plugin; }

            DlPlugin(DlPlugin&&) = default;
            DlPlugin& operator=(DlPlugin&&) = default;

            DlPlugin(TPlugin const& other) = delete;
            TPlugin &operator=(TPlugin const& other) = delete;

            ~DlPlugin() {
                //we don't delete pPlugin because it will go out of scope on it's own when the dl is closed

                /*
                 * if multiple DlPlugins are created for the same file then closing here doesn't
                 * actually close the handle until the last one is closed
                 */
                if (handle) Private::dlclose(handle);
            }
    };

}
