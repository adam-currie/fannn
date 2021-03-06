#include "expression.h"
#include <map>
#include <cmath>

using namespace Fannn;
using namespace std;
using namespace Fannn::Expression;

typedef struct ArithmeticOp : public function<double(double, double)>{
    const int priority;
    ArithmeticOp(int priority, function<double(double,double)> f) : function<double(double, double)>(f), 
        priority(priority) {}
} ArithmeticOp;

//todo: modulo
const map<char, ArithmeticOp> ARITHMETIC = {
    {'-', ArithmeticOp(1, minus<double>())}, 
    {'+', ArithmeticOp(1, plus<double>())},
    {'*', ArithmeticOp(2, multiplies<double>())},
    {'/', ArithmeticOp(2, divides<double>())},
    {'^', ArithmeticOp(3, [](double a,double b) {return pow(a,b);})}
};

const vector<char> Expression::RESERVED_SYMBOLS = []() { 
    vector<char> symbols;
    for(auto t: ARITHMETIC) symbols.push_back(t.first);
    symbols.push_back(')');
    symbols.push_back('(');
    return symbols;
}();

//returns false for failure including when outOfRange is set
bool tryParseConstant(string const & s, double & result, bool & outOfRange) {
    errno = 0;

    const char* cstr = s.c_str();
    char* endPtr;
    result = strtod(cstr, &endPtr);//todo: switch to from_chars when our compiler has compliant implementation

    ulong endIndex = endPtr - cstr;

    if (endIndex == s.length()) {
        return true;
    } else if (errno == ERANGE) {
        /*
         * todo:
         * i don't think gcc sets this or signals out of range in any way,
         * it just picks the closest aproximation, if this is okay which i think it is,
         * then we should probably just get rid of all this out of range stuff and
         * force it into range for compilers that don't do this for us.
         */
        outOfRange = true;
        return false;
    } else {
        outOfRange = false;
        return false;
    }

}

bool isArithmeticOp(string s) {
    return  s.size() == 1 &&
            ARITHMETIC.find(s.at(0)) != ARITHMETIC.end();
}

//doesn't tell you if it's a valid expression, just if its a good first token for one
bool isValidTokenToStartExpression(string const & token) {
    return token != ")" && !isArithmeticOp(token);
}

class Parser {
    vector<string> const & tokens;
    int tokenIndex = 0;

    string const & getToken(int n, string expected) {
        if (n >= tokens.size()) {
            string errMsg = "expected " + expected;
            if (n > 0) errMsg += " after '" + tokens[--n] + "'";
            throw TokenParseError(n, errMsg);
        }
        return tokens[n];
    }

    ArithmeticOp parseArithmeticOp() {
        string token = getToken(tokenIndex, "arithmetic operation");
        if(token.size() != 1) 
            throw TokenParseError::disappoint(tokenIndex, "arithmetic operation", token);
        
        auto opIter = ARITHMETIC.find(token.at(0));
        if (opIter == ARITHMETIC.end())
            throw TokenParseError::disappoint(tokenIndex, "arithmetic operation", token);

        tokenIndex++;
        return opIter->second;
    }

    /**
     * @brief parses a single expression without traversing operations
     *        (unless they are encapsulated within a function call or brackets)
     */
    ExecFunc parseSelfContainedExp() {
        string const & token = getToken(tokenIndex, "expression");

        if (!isValidTokenToStartExpression(token))
            throw TokenParseError::disappoint(tokenIndex, "expression", token);

        //CONSTANTS?
        bool outOfRange;
        double constant;
        if (tryParseConstant(token, constant, outOfRange)) {
            tokenIndex++;
            return [constant](INamedFuncContext const & context, NamedFuncErrorCallBack errorCallback, bool exhaustiveErrorChecking) {
                return constant;
            };
        } else if (outOfRange) {
            throw TokenParseError(tokenIndex, "constant value '" + token + "' is out of range");
        } else {
            // not a constant, keep looking
        }

        //BRACKETS?
        if(token == "(") {
            tokenIndex++;
            return parseExp(true);
        }

        //MUST BE NAMED FUNC THEN
        if (
            // determine if the func takes an arg
                tokens.size() > tokenIndex+1 &&
                isValidTokenToStartExpression(tokens[tokenIndex+1])) //not an arg if its not an expression
        {
            //named func takes an arg
            const int leftHandIndex = tokenIndex;
            tokenIndex++;
            ExecFunc rightHandExp = parseSelfContainedExp();
            return [token, rightHandExp, leftHandIndex](INamedFuncContext const & context, NamedFuncErrorCallBack errorCallback, bool exhaustiveErrorChecking) {
                double rightHandResult = rightHandExp(context, errorCallback, exhaustiveErrorChecking);
                bool rightHandExpressionFailed = isnan(rightHandResult);

                if (rightHandExpressionFailed && !exhaustiveErrorChecking) {
                    //don't need to call errorCallback because that's the sub-expressions job
                    return rightHandResult; //return the NaN value
                }

                double result;
                string err = {};//todo: maybe stop recreating all the time, not sure if this is optimized by the compiler
                bool success = rightHandExpressionFailed?
                        context.testLookUpOneArgFunc(token, err) :
                        context.lookupAndExec(token, result, err, rightHandResult);

                if (!success && errorCallback)
                    errorCallback(token, leftHandIndex, std::move(err), 1);

                return result;
            };
        } else {
            //named func takes zero args
            return [token, funcTokenIndex=tokenIndex++](INamedFuncContext const & context, NamedFuncErrorCallBack errorCallback, bool exhaustiveErrorChecking) {
                string err = {};//todo: maybe stop recreating all the time, not sure if this is optimized by the compiler
                double result;
                bool success = context.lookupAndExec(token, result, err);
                if (!success && errorCallback)
                    errorCallback(token, funcTokenIndex, std::move(err), 0);
                return result;
            };
        }
    }

    /**
     * @brief   traverses operations until we can be sure we can encapsulate 
     *          them in an expression without breaking order of operations, 
     *          then concatenates that expression with everything after
     * @param bracketed whether or not this expression segment is nested in brackets
     */
   ExecFunc parseExp(bool bracketed = false) {
        vector<ArithmeticOp> ops;
        vector<ExecFunc> subExpressions;

        auto endReached = [&]() {
            if (bracketed) {
                bool bracketHit = getToken(tokenIndex, "closing bracket") == ")";
                if (bracketHit) tokenIndex++;
                return bracketHit;
            } else {
                return tokenIndex >= tokens.size();
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
            ExecFunc everythingAfter = parseExp();
            return [subExpressions, ops, everythingAfter] (INamedFuncContext const & context, NamedFuncErrorCallBack errorCallback, bool exhaustiveErrorChecking) {
                double value = subExpressions.back()(context, errorCallback, exhaustiveErrorChecking);
                if (!exhaustiveErrorChecking && isnan(value)) 
                    return value;

                for (int i=ops.size()-2; i>=0; i--) {
                    value = ops[i](subExpressions[i](context, errorCallback, exhaustiveErrorChecking),value);
                    if (!exhaustiveErrorChecking && isnan(value)) 
                        return value;
                }

                return ops.back()(value, everythingAfter(context, errorCallback, exhaustiveErrorChecking));
            };
        } else {
            return [subExpressions, ops](INamedFuncContext const & context, NamedFuncErrorCallBack errorCallback, bool exhaustiveErrorChecking) {
                double value = subExpressions.back()(context, errorCallback, exhaustiveErrorChecking);
                for (int i=ops.size()-1; i>=0; i--) {
                    value = ops[i](subExpressions[i](context, errorCallback, exhaustiveErrorChecking),value);
                    if (!exhaustiveErrorChecking && isnan(value)) 
                        break;
                }
                return value;
            };
        }
    }

    public:
        Parser(vector<string> const & tokens)
            :   tokens(tokens) {}

        /**
         * @brief   parses the expression
         * 
         * @throws  TokenParseError thrown on failure
         * @return  ExecFunc the parsed expression if successful
         */
       ExecFunc parse() {
            if (tokens.empty())
                throw TokenParseError(0, "empty expression");
            return parseExp();
        }
};

Expression::ExecFunc Expression::parse(vector<string> const & tokens) {
    return Parser(tokens).parse();
}
