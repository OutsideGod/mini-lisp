#ifndef VALUE_H
#define VALUE_H

#include "./error.h"

#include <string>
#include <memory>
#include <vector>
#include <optional>

class Value;

using ValuePtr = std::shared_ptr<Value>;

class EvalEnv;

class Value {
public:
    virtual std::string toString() = 0;
    std::optional<std::string> asSymbol();
    bool isNumber();
    virtual double asNumber();
    virtual std::vector<ValuePtr> toVector();
    virtual ValuePtr Activatefunc(const std::vector<ValuePtr>&);
    virtual ~Value() = default;
};

using BuiltinFuncType = ValuePtr(const std::vector<ValuePtr>&);

class BooleanValue : public Value {
public:
    bool value;
    BooleanValue(bool);
    std::string toString();
};

class NumericValue : public Value {
public:
    double value;
    NumericValue(double);
    std::string toString();
    double asNumber();
};

class StringValue : public Value {
public:
    std::string value;
    StringValue(std::string);
    std::string toString();
};

class NilValue : public Value {
public:
    std::string toString();
};

class SymbolValue : public Value {
public:
    std::string value;
    SymbolValue(std::string);
    std::string toString();
};

class PairValue : public Value {
public:
    std::shared_ptr<Value> value1;
    std::shared_ptr<Value> value2;
    PairValue(std::shared_ptr<Value>, std::shared_ptr<Value>);
    std::string toString();
    std::vector<ValuePtr> toVector();
};

class BuiltinProcValue : public Value {
    BuiltinFuncType* func;
public:
    BuiltinProcValue(BuiltinFuncType*);
    std::string toString();
    ValuePtr Activatefunc(const std::vector<ValuePtr>&);
};

class LambdaValue : public Value {
private:
    std::vector<std::string> params;
    std::vector<ValuePtr> body;
    EvalEnv* env;
public:
    LambdaValue(std::vector<std::string>, std::vector<ValuePtr>);
    std::string toString();
    std::vector<std::string> getParams() const { return params; }
    std::vector<ValuePtr> getBody() const { return body; }
    EvalEnv* getEnv() const { return env; }
};
#endif
