#include "id_builder.h"

using namespace std;
using namespace Fannn;

IdBuilder& IdBuilder::operator<<(std::string s) {
    if (s.empty())
        return *this;

    if (hasData) {
        //add delim between strings
        out << DELIM;
    } else {
        //no delim needed before first string, next time though
        hasData = true;
    }

    for (auto const & c : s)
        if (!isspace(c))
            out << c;

    return *this;
}

IdBuilder& IdBuilder::operator<<(int n) {
    static const char digits[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};

    if (hasData) {
        //add delim between strings
        out << DELIM;
    } else {
        //no delim needed before first string, next time though
        hasData = true;
    }

    //convert to base 16,
    int digitCount = (31 - __builtin_clz(n))/4 + 1;
    for(int i = digitCount-1; i>=0; i--) {
        out << digits[n%16];
        n /= 16;
    }

    return *this;
}

string IdBuilder::get() const {
    return out.str();
}

