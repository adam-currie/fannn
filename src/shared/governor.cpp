#include "governor.h"
#include <sstream>
#include <variant>
#include <string.h>

using namespace std;

vector<string> splitOnWhiteSpace(string str){
    vector<string> v;
    istringstream iss(str);
    string word;
    while(iss >> word) v.push_back(word);
    return v;
}

bool tryParseNamedFunc(string s, function<function<int(int)>(string)> getNamedFunc, function<int(int)> &func){
    if (s.rfind(NAMED_FUNCTION_PREFIX, 0) == 0){
        string name = s.substr(NAMED_FUNCTION_PREFIX.length());
        func = getNamedFunc(name); //todo: handle not finding it
        return true;
    }else{
        return false;
    }
}

bool tryParseInput(string s, function<function<int()>(string)> getNamedInput, function<int()> &inputFunc){
    try {
        const int constant = stoi(s);
        inputFunc = [=](){ return constant; };
        return true;
    }catch(invalid_argument){
        if (s.rfind(NAMED_INPUT_PREFIX, 0) == 0){
            string name = s.substr(NAMED_INPUT_PREFIX.length());
            inputFunc = getNamedInput(name); //todo: handle not finding it
            return true;
        }else{
            return false;
        }
    }//todo: test how this fails with out_of_range exception
}

bool tryParseBivariantArithmetic(string s, function<int(int, int)> &func){
    if(s.length() != 1){
        return false;
    }

    switch(s[0]){
        case '+':
            func = plus<int>();
            return true;
        case '-':
            func = minus<int>();
            return true;
        case '*':
            func = multiplies<int>();
            return true;
        case '/':   //todo: division by zero
            func = divides<int>();
            return true;
        case '%':
            func = modulus<int>();
            return true;
        default:
            return false;
    }
}

function<int()> parseUserExp(
    string expression, 
    function<function<int()>(string)> getNamedInput,
    function<function<int(int)>(string)> getNamedFunc){
        vector<string> tokenStrs = splitOnWhiteSpace(expression);
        //the optional second parameter takes an input from inputs
        vector<variant<function<int(int)>, function<int(int,int)>>> ops;
        vector<function<int()>> inputs;

        //todo:bedmas
        for (string s : tokenStrs){
            function<int(int)> func;
            function<int(int, int)> bivariantFunc;
            function<int()> input;
            if (tryParseNamedFunc(s, getNamedFunc, func)){
                ops.push_back(func);
            }else if (tryParseBivariantArithmetic(s, bivariantFunc)){
                ops.push_back(bivariantFunc);
            }else if (tryParseInput(s, getNamedInput, input)){
                inputs.push_back(input);
            }
        }
        
        return [=](){ 
            auto input = inputs.begin();
            int r = (*input++)();

            for(auto op : ops){
                if(auto monovariantFunc = get_if<function<int(int)>>(&op)){
                    r = (*monovariantFunc)(r);
                }else{
                    auto byvariantFunc = get<function<int(int,int)>>(op);
                    r = byvariantFunc(r, (*input++)());
                }
            }
            return r; 
        };
}

template<typename T>
function<void()> GetGovernor(function<T()> getter, vector<function<void(T)>> setters){
    return [&]{//todo: mess around with value vs ref for both captured args
        T value = getter();
        for (auto setter : setters){
            setter(value);
        };
    };
}