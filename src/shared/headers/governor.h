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

                    Error() = default;

                    Error(std::string errMsg, std::vector<std::pair<int, int>> ranges)
                        : ranges(ranges), errMsg(errMsg) {}

                    bool operator==(const Error& e) const {
                        return  e.errMsg == errMsg;//intentionally ignoring ranges
                    }
            };

        private:
            std::string expStr;
            std::function<double()> exp;
            std::optional<Tokenizer> tokenizer;
            std::vector<Error> parseErrors;
            std::vector<Error> identifierErrors;
            std::map<std::string, std::vector<std::pair<int,int>>> zeroArgFuncTokens, oneArgFuncTokens;

        public:
            static const std::vector<char> RESERVED_SYMBOLS;

            std::string name;

            Governor(std::string name = "", std::string exp = "") : name(name) {
                setExpression(exp);
            }

            bool operator==(const Governor& g) const { 
                return  g.expStr == expStr &&
                        g.name == name &&
                        g.parseErrors == parseErrors &&
                        g.identifierErrors == identifierErrors;
            }

            std::vector<Error> const getErrors() const {
                //todo: good target for improving performance, this should be cached, or use iterators or something
                std::vector<Error> all;
                all.reserve(parseErrors.size() + identifierErrors.size());
                all.insert(all.end(), parseErrors.begin(), parseErrors.end());
                all.insert(all.end(), identifierErrors.begin(), identifierErrors.end());
                return all;
            }
            std::vector<Error> const & getParseErrors() const { return parseErrors; }
            std::vector<Error> const & getIdentifierErrors() const { return identifierErrors; }

            std::function<double(std::string)> readSensorOrGovernor;
            std::function<std::function<double(double)>(std::string)> readCurve;

            void setExpression(std::string userExpression);
            std::string const & getExpressionStr() const { return expStr; }

            void validateNameLookups(std::function<bool(std::string)> validateCurve, std::function<bool(std::string)> validateSensorOrGovernor);

            double exec();
    };

}
