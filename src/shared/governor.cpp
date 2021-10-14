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

struct _TokenParseError: public runtime_error {
    public:
        int tokenIndex;
        string errMsg;
        _TokenParseError(int tokenIndex, string errMsg) : runtime_error(errMsg),
            tokenIndex(tokenIndex), errMsg(errMsg) {}
};

class Parser{
    Governor *owner;
    function<bool(string)> validateCurve;
    function<bool(string)> validateSensorOrGovernor;
    vector<string> tokens;
    int n = 0;

    string getToken(int n, string errMsg){
        if (n >= tokens.size()) 
            throw _TokenParseError(n, errMsg);
        return tokens[n];
    }

    bool isArithmeticOp(string s){
        return  s.size() == 1 &&
                ARITHMETIC.find(s.at(0)) != ARITHMETIC.end();
    }

    ArithmeticOp parseArithmeticOp() {
        string token = getToken(n, "expected arithmetic operation");
        if(token.size() != 1) 
            throw  _TokenParseError(n, "expected arithmetic operation");
        
        auto opIterator = ARITHMETIC.find(token.at(0));
        if (opIterator == ARITHMETIC.end())
            throw _TokenParseError(n, "expected arithmetic operation");

        n++;
        return opIterator->second;
    }

    /**
     * @brief parses a single expression without traversing operations
     *        (unless they are encapsulated within a function call or brackets)
     */
    function<double()> parseSelfContainedExp() {
        string token = getToken(n, "expected an expression");

        if(isArithmeticOp(token))
            throw _TokenParseError(n, "expected an expression"); 

        //CONSTANTS?
        try {
            const double constant = stod(token);
            n++;
            return [=](){ return constant; };
        }catch(invalid_argument){
            //so its not a constant, fine by me!
        }catch(out_of_range){
            throw _TokenParseError(n, "constant value is out of range");
        }

        //BRACKETS?
        if(token == "("){
            n++;
            return parseExp(true);
        }

        //SENSORS, GOVERNORS AND CURVES?
        if (tokens.size() > n+1 && !isArithmeticOp(tokens[n+1])){
            //should be a curve because it takes an argument
            if(!validateCurve(token))
                throw _TokenParseError(n, "no curve matching: '" + token + "'"); 

            n++;
            auto rightHandExp = parseSelfContainedExp();
            return [*this, token, rightHandExp](){ 
                return owner->readCurve(token)(rightHandExp()); }
            ;
        } else {
            if(!validateSensorOrGovernor(token))
                throw _TokenParseError(n, "no sensor or governor matching: '" + token + "'");
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
                bool bracketHit = getToken(n, "expected closing bracket") == ")";
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
        Parser(vector<string> tokens, function<bool(string)> validateCurve, function<bool(string)> validateSensorOrGovernor, Governor *owner) 
            :   tokens(tokens), validateCurve(validateCurve), validateSensorOrGovernor(validateSensorOrGovernor), owner(owner) {}

        /**
         * @brief parses the expression
         * @throws _TokenParseError thrown on failure
         * @return function<double()> the parsed expression if successful
         */
        function<double()> parse(){
            return parseExp();
        }
};

void Governor::setExpression(string userExpression, function<bool(string)> validateCurve, function<bool(string)> validateSensorOrGovernor) {
    expStr = userExpression;//todo: what state do we leave everything in when this fails?

    Tokenizer tokenizer(userExpression, DELIMINATORS, RESERVED_SYMBOLS);

    try {
        exp = Parser(tokenizer.getTokens(), validateCurve, validateSensorOrGovernor, this)
                .parse();
    } catch(_TokenParseError e) {
        int start,end;
        try{
            tie(start,end) = tokenizer.backtraceToken(e.tokenIndex);
        } catch(out_of_range) {
            //probably our original error is that our token index went out of range!
            //in any case this means that our error applies to the whole dang string
            start = 0;
            end = userExpression.size()-1;
        }
        throw Governor::ParseError(userExpression, start, end, e.errMsg);
    }
}

double Governor::exec(){
    return exp();
}
