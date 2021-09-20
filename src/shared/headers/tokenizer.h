#pragma once

#include <vector>
#include <string>
#include <map>

class Tokenizer {
    std::string str;
    std::vector<std::string> tokens;
    std::map<int,std::tuple<int,int>> backtraceMap;
    public:
        /**
         * @brief Construct a new Tokenizer object
         * 
         * @param str string to tokenize
         * @param excludedDelims deliminators that are not tokenized
         * @param includedDelims deliminators that are also included in the list of tokens
         */
        Tokenizer(std::string str, std::vector<char> excludedDelims, std::vector<char> includedDelims);

        std::vector<std::string> getTokens() { return tokens; }
        std::string getStr() { return str; }

        /**
         * @brief finds a token in it's original position in the input string
         * 
         * @param tokenIndex 
         * @return tuple<int,int> {start,end} indexes of token in original string
         */
        std::tuple<int,int> backtraceToken(int tokenIndex);
};