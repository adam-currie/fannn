#pragma once

#include <string>
#include <functional>
#include <vector>
#include <stdexcept>
#include <memory>
#include <tokenizer.h>
#include <optional>
#include <map>
#include "expression.h"

namespace Fannn {

    class Governor {
        public:
            class Error {
                public:
                    std::vector<std::pair<int, int>> ranges;
                    std::string errMsg;
                    std::string token;

                    Error() = default;

                    Error(std::string token, std::string errMsg, std::vector<std::pair<int, int>> ranges)
                        : token(token), ranges(ranges), errMsg(errMsg) {}
            };

        private:
            std::string expStr;
            Fannn::Expression::ExecFunc exp;
            std::optional<Tokenizer> tokenizer;
            std::vector<Error> parseErrors;
            std::vector<Error> sensorAndGovErrors;
            std::vector<Error> curveErrors;

            //this changes under const context!
            bool __isExecuting = false;

        public:
            std::string name;

            Governor(std::string name = "", std::string expStr = "");

            //doesn't check transient stuff like execution errors
            bool operator==(const Governor& g) const { 
                return  g.expStr == expStr &&
                        g.name == name;
            }

            std::vector<Error> const getErrors() const {
                //todo: good target for improving performance, this should be cached, or use iterators or something
                std::vector<Error> all;
                all.reserve(parseErrors.size() + sensorAndGovErrors.size() + curveErrors.size());
                all.insert(all.end(), parseErrors.begin(), parseErrors.end());
                all.insert(all.end(), sensorAndGovErrors.begin(), sensorAndGovErrors.end());
                all.insert(all.end(), curveErrors.begin(), curveErrors.end());
                return all;
            }
            std::vector<Error> const & getParseErrors() const { return parseErrors; }
            std::vector<Error> const getExecutionErrors() const { 
                //todo: good target for improving performance, this should be cached, or use iterators or something
                std::vector<Error> errors;
                errors.reserve(sensorAndGovErrors.size() + curveErrors.size());
                errors.insert(errors.end(), sensorAndGovErrors.begin(), sensorAndGovErrors.end());
                errors.insert(errors.end(), curveErrors.begin(), curveErrors.end());
                return errors;    
            }

            void clearExecutionErrors();

            std::string const & getExpressionStr() const { return expStr; }

            /**
             * @brief   computes the output of the governor
             * 
             * @note    clears old execution errors
             * @param   context user for looking up named functions in the governors expression
             * @param   exhaustiveErrorChecking 
             *              set to true to keep looking for more errors even after we hit the first one
             * @return  the value or NaN in case of error
             */
            double exec(Fannn::Expression::INamedFuncContext const & context, bool exhaustiveErrorChecking = true);

            /**
             * @brief   computes the output of the governor but doesn't update errors
             *
             * @note    does not clear execution errors
             * @param   context user for looking up named functions in the governors expression
             * @return  the value or NaN in case of error
             */
            double constExec(Fannn::Expression::INamedFuncContext const & context) const;
    };

}
