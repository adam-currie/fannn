#pragma once

#include <optional>
#include <cassert>

namespace Fannn::Plugins::Internal {

    /**
     * @brief RAII wrapper around dlopen stuff
     */
    class DlObj {
        void* handle;//shall only be null if moved from

        DlObj(void* handle) : handle(handle) {
            assert(handle);
        }

        public:
            static std::optional<DlObj> open(const char * const path, const char * err);

            //this returns null on failure but also if the symbol is defined as null(see implementation, easy to change)
            void* getSymbol(const char * const symbolName);

            void* rawHandle() {
                return handle;
            }

            bool operator==(const DlObj &other) const = default;

            DlObj(DlObj&& other) {
                assert(other.handle);
                handle = other.handle;
                other.handle = nullptr;
            }

            DlObj(DlObj const& other) = delete;
            DlObj& operator=(DlObj&& other) = delete;
            DlObj &operator=(DlObj const& other) = delete;

            ~DlObj();
    };

}
