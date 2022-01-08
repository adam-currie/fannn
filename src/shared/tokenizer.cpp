#include "tokenizer.h"
#include <algorithm>
#include "containers_util.h"

using namespace std;
using namespace Fannn;

Tokenizer::Tokenizer(string str, vector<char> const & excludedDelims, vector<char> const & includedDelims)
  : str(str) {

    int i = 0;
    int startOfToken = -1;

    auto addToken = [&](int start, int end){
        backtraceMap.push_back({start,end});
        tokens.push_back(str.substr(start, end-start+1));
    };

    auto endCurrentToken = [&](){
        if (startOfToken != -1){
            addToken(startOfToken, i-1);
            startOfToken = -1;
        }
    };

    using Fannn::Util::contains;

    for (; i<str.size(); i++) {
        if (excludedDelims>>contains(str[i])) {
            endCurrentToken();
        } else if (includedDelims>>contains(str[i])) {
            endCurrentToken();
            addToken(i,i);
        } else if (startOfToken == -1){
            startOfToken = i;
        }
    }

    endCurrentToken();

}

pair<int,int> Tokenizer::backtraceToken(int tokenIndex){
    return backtraceMap.at(tokenIndex);
}
