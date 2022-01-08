#pragma once

#include <string>
#include <memory>
#include <experimental/propagate_const>

namespace Fannn::Util {

    class AtomicFileWriter {
        class Impl;
        std::experimental::propagate_const<std::unique_ptr<Impl>> pImpl;
        public:
            AtomicFileWriter(std::string filename);
            ~AtomicFileWriter();
            void atomicWrite();
            AtomicFileWriter& operator<<(const char* s);
            AtomicFileWriter& operator<<(const std::string& s) { 
                return operator<<(s.c_str());
            }
    };

}
