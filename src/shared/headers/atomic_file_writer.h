#pragma once

#include <string>
#include <memory>
#include <experimental/propagate_const>

namespace Fannn {

    class AtomicFileWriter{
        class Impl;
        std::experimental::propagate_const<std::unique_ptr<Impl>> pImpl;
        public:
            AtomicFileWriter(std::string filename);
            ~AtomicFileWriter();
            void atomicWrite();
    };

}