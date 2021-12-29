#pragma once

namespace Fannn {

    class ReentrancyBlocker {
        bool entered = false;
        bool failFlag = false;

        public:
            template<typename F>
            bool enter(F fn) {
                if (entered) {
                    failFlag = true;
                    return false;
                }

                entered = true;

                try {
                    fn();
                    entered = false;
                    return true;
                }  catch (...) {
                    entered = false;
                    throw;
                }
            }

            bool checkFailFlag() {
                return failFlag;
            }

            bool resetFailFlag() {
                bool old = failFlag;
                failFlag = false;
                return old;
            }
        };

}
