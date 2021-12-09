#pragma once

#include <vector>
#include <string>
#include <stdexcept>
#include <functional>

namespace Fannn {

    namespace Expression {

        extern const std::vector<char> RESERVED_SYMBOLS;

        struct TokenParseError: public std::runtime_error {
            public:
                int tokenIndex;
                std::string errMsg;
                TokenParseError(int tokenIndex, std::string errMsg) : std::runtime_error(errMsg),
                    tokenIndex(tokenIndex), errMsg(errMsg) {}
                static TokenParseError disappoint(int tokenIndex, std::string wanted, std::string got) {
                    return TokenParseError(tokenIndex, "expected " + wanted + ", not '" + got + "'");
                }
        };

        class INamedFuncContext {
            public:
                virtual double lookupAndExec(const std::string& id, std::string & errMsg) const = 0;
                virtual double lookupAndExec(const std::string& id, std::string & errMsg, double arg) const = 0;
        };

        typedef std::function <
            //argCount tells you how many arguments this identifier was supposed to take
            void (std::string identifier, int tokenIndex, std::string const errMsg, int argCount)
        > NamedFuncErrorCallBack;

        typedef std::function <
            //IdentifierErrorCallback can be empty
            double (INamedFuncContext const &, NamedFuncErrorCallBack IdentifierErrorCallback, bool exhaustiveErrorChecking)
        > ExecFunc;

        /**
         * @brief turns a user expression into a function
         * 
         * @param tokens tokens to parse
         * @return ExecFunc the parsed expression
         * @exception TokenParseError tells you which token messed us up and why
         */
        ExecFunc parse(std::vector<std::string> const & tokens);
    }

}
