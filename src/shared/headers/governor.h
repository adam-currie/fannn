#pragma once

#include <string>
#include <functional>
#include <vector>
#include <stdexcept>
#include <memory>

namespace Fannn {

    class Governor {
        private:
            std::string expStr;
            std::function<double()> exp;
        public:
            class ParseError: public std::runtime_error {
                public:
                    std::string exp;
                    int startIndex, endIndex;
                    std::string errMsg;
                    ParseError(std::string exp, int startIndex, int endIndex, std::string errMsg) :
                        std::runtime_error(errMsg),
                        exp(exp), startIndex(startIndex), endIndex(endIndex), errMsg(errMsg) {}
            };
        
            static const std::vector<char> RESERVED_SYMBOLS;

            bool operator==(const Governor& g) const { 
                return g.expStr == expStr; 
            }

            std::function<double(std::string)> readSensorOrGovernor;
            std::function<std::function<double(double)>(std::string)> readCurve;
            
            std::string name() { return "todo"; }

            /**
             * @brief Construct a new Governor object
             * 
             * @exception ParseError //todo
             */
            Governor(std::string userExpression, std::function<bool(std::string)> validateCurve, std::function<bool(std::string)> validateSensorOrGovernor);

            double exec();
    };

}
