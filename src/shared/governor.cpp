#include "governor.h"
#include <sstream>
#include <variant>
#include <string.h>
#include <cmath>
#include <assert.h>
#include "tokenizer.h"

using namespace std;
using namespace Fannn;

const vector<char> DELIMINATORS = {' ', '\f', '\n', '\r', '\t', '\v'};

Governor::Governor(string name, string expStr) : name(name), expStr(expStr) {
    parseErrors.clear();
    clearExecutionErrors();

    tokenizer = Tokenizer(expStr, DELIMINATORS, Expression::RESERVED_SYMBOLS);

    try {
        exp = Expression::parse(tokenizer->getTokens());
    } catch (Expression::TokenParseError e) {
        int start, end;
        try{
            tie(start,end) = tokenizer->backtraceToken(e.tokenIndex);
        } catch(out_of_range) {
            //error doesn't point to specific token
            start = -1;
            end = -1;
        }
        string tokenStr = (e.tokenIndex >= 0 && e.tokenIndex < tokenizer->count()) ?
                (*tokenizer)[e.tokenIndex] :
                "";
        parseErrors.push_back(Error(tokenStr, e.errMsg, {{start, end}}));//todo: emplace
    }
}

void Governor::clearExecutionErrors() {
    curveErrors.clear();
    sensorAndGovErrors.clear();
}

double Governor::exec(Fannn::Expression::INamedFuncContext const & context, bool exhaustiveErrorChecking) {
    clearExecutionErrors();

    if (!parseErrors.empty()) {
        return numeric_limits<double>::quiet_NaN();
    }

    auto errorCallback = [this] (string identifier, int tokenIndex, string errMsg, int argCount) {
        if (reentrancyBlocker.resetFailFlag())
            errMsg = "infinite self reference";

        auto range = tokenizer->backtraceToken(tokenIndex);

        auto & errors = (argCount == 0) ?
            sensorAndGovErrors : curveErrors;

        for (auto & e : errors) {
            if (e.token == identifier) {
                e.ranges.push_back(range);
                return;
            }
        }
        
        //should only be reached if we couldnt find a pre-existing error for this identifier
        errors.push_back(Error(identifier, errMsg, {range}));
    };

    double result;
    bool entered = reentrancyBlocker.enter([&]{
        result = exp(context, errorCallback, exhaustiveErrorChecking);
    });
    return entered? result : numeric_limits<double>::quiet_NaN();
}

double Governor::constExec(Fannn::Expression::INamedFuncContext const & context) const {
    if (!parseErrors.empty()) {
        return numeric_limits<double>::quiet_NaN();
    }

    double result;
    bool entered = reentrancyBlocker.enter([&]{
        result = exp(context, {}, true);
    });
    return entered? result : numeric_limits<double>::quiet_NaN();
}
