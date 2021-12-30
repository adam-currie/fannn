#pragma once

template<typename T>
class contains {
    const T& item;

    public:
        contains(const T& item) : item(item) {}

        template<typename C>
        friend const bool operator>>(const C& container, const contains& args) {
            for (const auto& i : container)
                if (i == args.item)
                    return true;
            return false;
        }
};