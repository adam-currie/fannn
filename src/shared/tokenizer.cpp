#include "tokenizer.h"
#include <algorithm>

using namespace std;
using namespace Fannn;

template <typename T>
bool contains(vector<T> v, T t){
    return find(v.begin(), v.end(), t) != v.end();
}

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

    for (; i<str.size(); i++) {
        if (contains(excludedDelims, str[i])) {
            endCurrentToken();
        }else if (contains(includedDelims, str[i])) {
            endCurrentToken();
            addToken(i,i);
        }else if(startOfToken == -1){
            startOfToken = i;
        }
    }

    endCurrentToken();

}

pair<int,int> Tokenizer::backtraceToken(int tokenIndex){
    return backtraceMap.at(tokenIndex);
}
