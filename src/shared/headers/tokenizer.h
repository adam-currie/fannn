#pragma once

#include <vector>
#include <string>

namespace Fannn {

    class Tokenizer {
        std::string str;
        std::vector<std::string> tokens;
        std::vector<std::pair<int,int>> backtraceMap;

        public:
            /**
             * @brief Construct a new Tokenizer object
             * 
             * @param str string to tokenize
             * @param excludedDelims deliminators that are not tokenized
             * @param includedDelims deliminators that are also included in the list of tokens
             */
            Tokenizer(std::string str, std::vector<char> excludedDelims, std::vector<char> includedDelims);

            std::vector<std::string> const & getTokens() const { return tokens; }
            std::string getStr() { return str; }

            /**
             * @brief finds a token in it's original position in the input string
             * 
             * @param tokenIndex 
             * @return pair<int,int> {start,end} indexes of token in original string
             */
            std::pair<int,int> backtraceToken(int tokenIndex);
    };

}
