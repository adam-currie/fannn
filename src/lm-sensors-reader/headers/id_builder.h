#pragma once

#include <sstream>

namespace Fannn {

    class IdBuilder {
        const char DELIM = '\\';

        std::ostringstream out;
        bool hasData = false;

        public:
            IdBuilder() {}

            IdBuilder& operator<<(std::string s);
            IdBuilder& operator<<(int n);
            std::string get() const;
    };

}
