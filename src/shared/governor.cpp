#include "governor.h"
#include <sstream>
#include <variant>
#include <string.h>
#include "tokenizer.h"
#include <math.h>

using namespace std;
using namespace Fannn;

const vector<char> DELIMINATORS = {' ', '\f', '\n', '\r', '\t', '\v'};

typedef struct ArithmeticOp : public function<double(double, double)>{
    const char symbol;
    const int priority;
    ArithmeticOp() : symbol(0), priority(0){}
    ArithmeticOp(char symbol, int priority, function<double(double,double)> f) : function<double(double, double)>(f), 
        symbol(symbol), priority(priority) {}
} ArithmeticOp;

const map<char, ArithmeticOp> ARITHMETIC = {
    {'-', ArithmeticOp('-', 1, minus<double>())}, 
    {'+', ArithmeticOp('+', 1, plus<double>())},
    {'*', ArithmeticOp('*', 2, multiplies<double>())},
    {'/', ArithmeticOp('/', 2, divides<double>())},
    {'^', ArithmeticOp('^', 3, [](double a,double b){return pow(a,b);})}
};

const vector<char> Governor::RESERVED_SYMBOLS = [](){ 
    vector<char> symbols;
    for(auto t: ARITHMETIC) symbols.push_back(t.first);
    symbols.push_back(')');
    symbols.push_back('(');
    return symbols;
}();

template <typename T>
bool contains(vector<T> v, T t){
    return find(v.begin(), v.end(), t) != v.end();
}

bool isArithmeticOp(string s){
    return  s.size() == 1 &&
            ARITHMETIC.find(s.at(0)) != ARITHMETIC.end();
}

struct _TokenParseError: public runtime_error {
    public:
        int tokenIndex;
        string errMsg;
        _TokenParseError(int tokenIndex, string errMsg) : runtime_error(errMsg),
            tokenIndex(tokenIndex), errMsg(errMsg) {}
        static _TokenParseError disappoint(int tokenIndex, string wanted, string got){
            return _TokenParseError(tokenIndex, "expected " + wanted + ", not '" + got + "'");
        }
};

class Parser{
    Governor *owner;
    vector<string> tokens;
    int n = 0;

    string getToken(int n, string expected){
        if (n >= tokens.size()) {
            string errMsg = "expected " + expected;
            if (n > 0) errMsg += " after '" + tokens[--n] + "'";
            throw _TokenParseError(n, errMsg);
        }
        return tokens[n];
    }

    ArithmeticOp parseArithmeticOp() {
        string token = getToken(n, "arithmetic operation");
        if(token.size() != 1) 
            throw _TokenParseError::disappoint(n, "arithmetic operation", token);
        
        auto opIterator = ARITHMETIC.find(token.at(0));
        if (opIterator == ARITHMETIC.end())
            throw _TokenParseError::disappoint(n, "arithmetic operation", token);

        n++;
        return opIterator->second;
    }



    /**
     * @brief parses a single expression without traversing operations
     *        (unless they are encapsulated within a function call or brackets)
     */
    function<double()> parseSelfContainedExp() {
        string token = getToken(n, "expression");

        if(isArithmeticOp(token))
            throw _TokenParseError::disappoint(n, "expression", token);

        //CONSTANTS?
        try {
            const double constant = stod(token);
            n++;
            return [=](){ return constant; };
        }catch(invalid_argument){
            //so its not a constant, fine by me!
        }catch(out_of_range){
            throw _TokenParseError(n, "constant value '" + token + "' is out of range");
        }

        //BRACKETS?
        if(token == "("){
            n++;
            return parseExp(true);
        }

        //SENSORS, GOVERNORS AND CURVES?
        if (tokens.size() > n+1 && !isArithmeticOp(tokens[n+1])){
            //should be a curve because it takes an argument
            n++;
            auto rightHandExp = parseSelfContainedExp();
            return [*this, token, rightHandExp](){ 
                return owner->readCurve(token)(rightHandExp()); }
            ;
        } else {
            //should be a sensor or governor
            n++;
            return [*this, token](){
                 return owner->readSensorOrGovernor(token); 
            };
        }
    }

    /**
     * @brief   traverses operations until we can be sure we can encapsulate 
     *          them in an expression without breaking order of operations, 
     *          then concatenates that expression with everything after
     */
    function<double()> parseExp(bool bracketed = false) {
        vector<ArithmeticOp> ops;
        vector<function<double()>> subExpressions;

        auto endReached = [&](){
            if(bracketed){
                bool bracketHit = getToken(n, "closing bracket") == ")";
                if (bracketHit) n++;
                return bracketHit;
            }else{
                return n >= tokens.size();
            }
        };

        //loop through expression+op pairs until we are sure we can encapsulate them without breaking order of operations
        do {
            subExpressions.push_back(parseSelfContainedExp());
            if (endReached())
                break;
            ops.push_back(parseArithmeticOp());
        } while (
            ops.size() < 2 || 
            ops.back().priority >= ops[ops.size()-2].priority
        );

        if (ops.size() == subExpressions.size()) {
            //if we end in an op then we need to keep parsing
            //concatenate the expression we encapsulated with everything after
            function<double()> everythingAfter = parseExp();
            return [=](){ 
                double value = subExpressions.back()();
                for (int i=ops.size()-2; i>=0; i--)
                    value = ops[i](subExpressions[i](),value);
                return ops.back()(value, everythingAfter());
            };
        }else{
            return [=](){ 
                double value = subExpressions.back()();
                for (int i=ops.size()-1; i>=0; i--)
                    value = ops[i](subExpressions[i](),value);
                return value;
            };
        }
    }

    public:
        Parser(vector<string> tokens, Governor *owner)
            :   tokens(tokens), owner(owner) {}

        /**
         * @brief parses the expression
         * @throws _TokenParseError thrown on failure
         * @return function<double()> the parsed expression if successful
         */
        function<double()> parse(){
            if (tokens.empty())
                throw _TokenParseError(0, "empty expression");
            return parseExp();
        }
};

void Governor::setExpression(string userExpression) {
    expStr = userExpression;//todo: what state do we leave everything in when this fails?

    errors.clear();

    tokenizer = Tokenizer(userExpression, DELIMINATORS, RESERVED_SYMBOLS);

    zeroArgFuncTokens = {};
    oneArgFuncTokens = {};
    auto const & tokens = tokenizer->getTokens();
    for (int i=0; i<tokens.size(); i++) {
        string tokenStr = tokens[i];

        try {
            const double constant = stod(tokenStr);
            //constant
            continue;
        }catch(invalid_argument){
            //not constant
        }

        if ( tokenStr.size() > 1 || !contains(RESERVED_SYMBOLS, tokenStr[0]) ) {
            if (tokens.size() > i+1 && !isArithmeticOp(tokens[i+1])){
                //should be curve
                auto indexes = oneArgFuncTokens[tokenStr];
                indexes.push_back(tokenizer->backtraceToken(i));
                oneArgFuncTokens[tokenStr] = indexes;
            } else {
                //should be a sensor or governor
                auto indexes = zeroArgFuncTokens[tokenStr];
                indexes.push_back(tokenizer->backtraceToken(i));
                zeroArgFuncTokens[tokenStr] = indexes;
            }
        }
    }

    try {
        exp = Parser(tokenizer->getTokens(), this)
                .parse();
    } catch(_TokenParseError e) {
        int start,end;
        try{
            tie(start,end) = tokenizer->backtraceToken(e.tokenIndex);
        } catch(out_of_range) {
            //error doesn't point to specific token
            start = -1;
            end = -1;
        }
        errors.push_back(Error(e.errMsg, {{start, end}}));
    }
}

void Governor::validateNameLookups(std::function<bool (std::string)> validateCurve, std::function<bool (std::string)> validateSensorOrGovernor) {
    for (auto const & [identifier, ranges] : zeroArgFuncTokens){
        if (name == identifier) {
            errors.push_back(Error("infinite self-reference", ranges));
        } else if (!validateSensorOrGovernor(identifier)) {
            errors.push_back(Error("no governor or sensor named '" + identifier + "'", ranges));
        }
    }
    for (auto const & [identifier, ranges] : oneArgFuncTokens)
        if (!validateCurve(identifier))
            errors.push_back(Error("no curve named '" + identifier + "'", ranges));
}

double Governor::exec(){
    return exp();
}
