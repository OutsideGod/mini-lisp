#include "./value.h"
#include <memory>

BooleanValue::BooleanValue(bool value) : value{value} {}
NumericValue::NumericValue(double value) : value{value} {}
StringValue::StringValue(std::string value) : value{std::string(value)} {}
SymbolValue::SymbolValue(std::string value) : value{std::string(value)} {}
PairValue::PairValue(std::shared_ptr<Value> a, std::shared_ptr<Value> b) : value1{a}, value2{b} {}

std::string BooleanValue::toString() {
    if (value) {
        return "#t";
    } 
    else {
        return "#f";
    }
}
std::string NumericValue::toString() {
    return std::to_string(int(value));
}
std::string StringValue::toString() {
    int a = value.size();
    std::string out = "";
    out.append(1, '"');
    for (int i = 0; i < a; i++) {
        if (value[i] == '"') {
            out += R"(\)";
            out.append(1, '"');
        } 
        else {
            out.append(1, value[i]);
        }
    }
    out.append(1, '"');
    return out;
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
    
}
