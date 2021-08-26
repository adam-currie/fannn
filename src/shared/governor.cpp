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

template <typename T, typename enable_if<is_arithmetic<T>::value>::type* = nullptr>
bool tryParseNamedFunc(string s, function<function<T(T)>(string)> getNamedFunc, function<T(T)> &func){
    if (s.rfind(NAMED_FUNCTION_PREFIX, 0) == 0){
        string name = s.substr(NAMED_FUNCTION_PREFIX.length());
        func = getNamedFunc(name); //todo: handle not finding it
        return true;
    }else{
        return false;
    }
}

template <typename T, typename enable_if<is_arithmetic<T>::value>::type* = nullptr>
bool tryParseInput(string s, function<function<T()>(string)> getNamedInput, function<T()> &inputFunc){
    try {
        const T constant = stoi(s);
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

template <typename T, typename enable_if<is_arithmetic<T>::value>::type* = nullptr>
bool tryParseBivariantArithmetic(string s, function<T(T, T)> &func){
    if(s.length() != 1){
        return false;
    }

    switch(s[0]){
        case '+':
            func = plus<T>();
            return true;
        case '-':
            func = minus<T>();
            return true;
        case '*':
            func = multiplies<T>();
            return true;
        case '/':   //todo: division by zero
            func = divides<T>();
            return true;
        default:
            return false;
    }
}

//todo: all other arithmetic types except maybe bool
template function<int()> parseUserExp<int>(string, function<function<int()>(string)>, function<function<int(int)>(string)>);
template function<double()> parseUserExp<double>(string, function<function<double()>(string)>, function<function<double(double)>(string)>);
template <typename T>
function<T()> parseUserExp(
    string expression, 
    function<function<T()>(string)> getNamedInput,
    function<function<T(T)>(string)> getNamedFunc)
    {
    vector<string> tokenStrs = splitOnWhiteSpace(expression);//todo: quote strings to allow for spaces in them

    //ops optionally take a second parameter that takes an input from input
    vector<variant<function<T(T)>, function<T(T,T)>>> ops;
    vector<function<T()>> inputs;

    //todo:bedmas
    for (string s : tokenStrs){
        function<T(T)> func;
        function<T(T, T)> bivariantFunc;
        function<T()> input;
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
            if(auto monovariantFunc = get_if<function<T(T)>>(&op)){
                r = (*monovariantFunc)(r);
            }else{
                auto byvariantFunc = get<function<T(T,T)>>(op);
                r = byvariantFunc(r, (*input++)());
            }
        }
        return r; 
    };
}

//todo: all other arithmetic types except maybe bool
template function<void()> getGovernor<int>(function<int()>, vector<function<void(int)>>);
template function<void()> getGovernor<double>(function<double()>, vector<function<void(double)>>);
template <typename T>
function<void()> getGovernor(function<T()> getter, vector<function<void(T)>> setters){
    return [=]{
        T value = getter();
        for (auto setter : setters){
            setter(value);
        };
    };
}