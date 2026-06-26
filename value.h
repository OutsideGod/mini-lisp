#ifndef VALUE_H
#define VALUE_H

// 你的代码
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "./error.h"

class Value;
class EvalEnv;

using ValuePtr = std::shared_ptr<Value>;

class Value {
public:
    virtual std::string toString() = 0;
    std::optional<std::string> asSymbol();
    bool isNumber();
    virtual double asNumber();
    virtual bool asBool();
    virtual std::vector<ValuePtr> toVector();
    virtual ValuePtr Activatefunc(const std::vector<ValuePtr>&);
    virtual ~Value() = default;
};

using BuiltinFuncType = ValuePtr(const std::vector<ValuePtr>&);

class BooleanValue : public Value {
public:
    bool value;
    BooleanValue(bool value);
    std::string toString();
    bool asBool();
};
class NumericValue : public Value {
public:
    double value;
    NumericValue(double value);
    std::string toString();
    double asNumber();
};
class StringValue : public Value {
public:
    std::string value;
    StringValue(std::string value);
    std::string toString();
};
class NilValue : public Value {
public:
    std::string toString();
};
class SymbolValue : public Value {
public:
    std::string value;
    SymbolValue(std::string value);
    std::string toString();
};
class PairValue : public Value {
public:
    std::shared_ptr<Value> value1;
    std::shared_ptr<Value> value2;
    PairValue(std::shared_ptr<Value> a, std::shared_ptr<Value> b);
    std::string toString();
    std::vector<ValuePtr> toVector();
    ValuePtr getCdr();
};
class BuiltinProcValue : public Value {
    BuiltinFuncType* func;

public:
    BuiltinProcValue(BuiltinFuncType* a);
    std::string toString();
    ValuePtr Activatefunc(const std::vector<ValuePtr>&);
};

class LambdaValue : public Value {
private:
    std::vector<std::string> params;
    std::vector<ValuePtr> body;
    std::shared_ptr<EvalEnv> env;
public:
    LambdaValue(std::vector<std::string>, std::vector<ValuePtr>, const std::shared_ptr<EvalEnv>&);
    std::string toString();
    ValuePtr apply(const std::vector<ValuePtr>&);
};
#endif
