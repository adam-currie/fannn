#include "dl_obj.h"

#include <dlfcn.h>

namespace Fannn::Plugins::Internal {

    std::optional<DlObj> DlObj::open(const char * const path, const char * err) {
        void* handle = dlopen(path, RTLD_NOW | RTLD_LOCAL | RTLD_DEEPBIND);
        if (handle) {
            return std::optional(std::move(DlObj(const_cast<void*>(handle))));
        } else {
            err = dlerror();
            return std::nullopt;
        }
    }

    void* DlObj::getSymbol(const char * const symbolName) {
        assert(handle);
        //call dlerror before and after dlsym if you want to change for errors even for symbols defined as null
        return dlsym(const_cast<void*>(handle), symbolName);
    }

    DlObj::~DlObj() {
        if (handle) {
            /*
             * if multiple dlobjs are created for the same file then closing here doesn't
             * actually close the handle until the last one is closed
             */
            dlclose(const_cast<void*>(handle));
        }
    }

}
