#pragma once

namespace Fannn::Util {

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

    template<typename T>
    class addAll {
        const T& item;

        public:
            addAll(const T& item) : item(item) {}

            template<typename C>
            friend void operator>>(C& container, const addAll& args) {
                container.insert(container.end(), args.item.begin(), args.item.end());
            }
    };

}
