#ifndef VALUE_H
#define VALUE_H

// 你的代码
#include <string>
#include <memory>

class Value {
public:
    virtual std::string toString() = 0;
};
class BooleanValue : public Value {
public:
    bool value;
    BooleanValue(bool value);
    std::string toString();
};
class NumericValue : public Value {
public:
    double value;
    NumericValue(double value);
    std::string toString();
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
};
#endif
