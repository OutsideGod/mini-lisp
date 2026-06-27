#include "value.h"
#include "eval_env.h"

#include <iomanip>
#include <memory>
#include <sstream>

std::optional<std::string> Value::asSymbol() {
    if (typeid(*this) == typeid(SymbolValue)) {
        return this->toString();
    } else {
        return std::nullopt;
    }
}
bool Value::isNumber() {
    if (typeid(*this) == typeid(NumericValue)) {
        return true;
    } else {
        return false;
    }
}
double Value::asNumber() {
    return 0;
}
bool Value::asBool() {
    return true;
}
std::vector<ValuePtr> Value::toVector() {
    return {};
}
ValuePtr Value::Activatefunc(const std::vector<ValuePtr>& args) {
    throw LispError("Attempt to call a non-procedure value");
}

BooleanValue::BooleanValue(bool value) : value{value} {}
NumericValue::NumericValue(double value) : value{value} {}
StringValue::StringValue(std::string value) : value{std::string(value)} {}
SymbolValue::SymbolValue(std::string value) : value{std::string(value)} {}
PairValue::PairValue(std::shared_ptr<Value> a, std::shared_ptr<Value> b)
    : value1{a}, value2{b} {}

bool BooleanValue::asBool() {
    return value;
}

std::string BooleanValue::toString() {
    if (value) {
        return "#t";
    } else {
        return "#f";
    }
}
std::string NumericValue::toString() {
    if (std::floor(value) == value) {
        return std::to_string(int(value));
    } else {
        return std::to_string(double(value));
    }
}
double NumericValue::asNumber() {
    return value;
}
std::string StringValue::toString() {
    std::stringstream ss;
    ss << std::quoted(value);
    return ss.str();
}
std::string NilValue::toString() {
    return "()";
}
std::string SymbolValue::toString() {
    return value;
}
std::string PairValue::toString() {
    std::string result = "(";
    result += value1->toString();
    PairValue* value = this;
    while (auto next_value =
               std::dynamic_pointer_cast<PairValue>(value->value2)) {
        result += " " + next_value->value1->toString();
        value = next_value.get();
    }
    if (typeid(*(value->value2)) != typeid(NilValue)) {
        result += " . " + value->value2->toString();
    }
    result += ")";
    return result;
}
std::vector<ValuePtr> PairValue::toVector() {
    std::vector<ValuePtr> result;
    result.push_back(this->value1);
    PairValue* value = this;
    while (auto next_value = std::dynamic_pointer_cast<PairValue>(value->value2)) {
        result.push_back(next_value->value1);
        value = next_value.get();
    }
    if (typeid(*(value->value2)) != typeid(NilValue)) {
        throw SyntaxError("SyntaxError");
    }
    return result;
}

ValuePtr PairValue::getCdr(){
    return this->value2;
}

BuiltinProcValue::BuiltinProcValue(BuiltinFuncType a) : func{std::move(a)} {}
std::string BuiltinProcValue::toString() {
    return "#<procedure>";
}

ValuePtr BuiltinProcValue::Activatefunc(const std::vector<ValuePtr>& args) {
    return func(args);
}

LambdaValue::LambdaValue(std::vector<std::string> a, std::vector<ValuePtr> b, const std::shared_ptr<EvalEnv>& c) : params{ a }, body{ b }, env { c } {}

std::string LambdaValue::toString() {
    return "#<procedure>";
}

ValuePtr LambdaValue::apply(const std::vector<ValuePtr>& args) {
    std::shared_ptr<EvalEnv> child_env = env->createChild(params, args);
    ValuePtr result = std::make_shared<NilValue>();
    for (auto element : body) {
        result = child_env->eval(element);
    }
    return result;
}
