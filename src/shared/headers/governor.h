#pragma once

#include <string>
#include <functional>
#include <vector>
#include <stdexcept>
#include <memory>
#include <tokenizer.h>
#include <optional>
#include <map>

namespace Fannn {

    class Governor {
        public:
            class Error {
                public:
                    std::vector<std::pair<int, int>> ranges;
                    std::string errMsg;
                    Error(std::string errMsg, std::vector<std::pair<int, int>> ranges)
                        : ranges(ranges), errMsg(errMsg) {}
            };

        private:
            std::string expStr;
            std::function<double()> exp;
            std::optional<Tokenizer> tokenizer;
            std::vector<Error> errors;
            std::map<std::string, std::vector<std::pair<int,int>>> zeroArgFuncTokens, oneArgFuncTokens;

        public:
            static const std::vector<char> RESERVED_SYMBOLS;

            std::string name;

            Governor() {}
            Governor(std::string name) : name(name) {}

            bool operator==(const Governor& g) const { 
                return  g.expStr == expStr &&
                        name == g.name;
            }

            std::vector<Error> const & getErrors() const { return errors; }

            std::function<double(std::string)> readSensorOrGovernor;
            std::function<std::function<double(double)>(std::string)> readCurve;

            void setExpression(std::string userExpression);
            std::string const & getExpressionStr() const { return expStr; }

            void validateNameLookups(std::function<bool(std::string)> validateCurve, std::function<bool(std::string)> validateSensorOrGovernor);

            double exec();
    };

}
