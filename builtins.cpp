#include "builtins.h"
#include <iostream>

ValuePtr add(const std::vector<ValuePtr>& params) {
    auto result = 0.0;
    for (const auto& i : params) {
        if (!i->isNumber()) {
            throw LispError("Cannot add a non-numeric value.");
        }
        result += i->asNumber();
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr sub(const std::vector<ValuePtr>& params) {
    if (params.empty()) {
        throw LispError("Too few arguments for subtraction.");
    }
    if (params.size() == 1) {
        if (!params[0]->isNumber()) {
            throw LispError("Cannot negate a non-numeric value.");
        }
        return std::make_shared<NumericValue>(-params[0]->asNumber());
    }
    if (!params[0]->isNumber()) {
        throw LispError("Cannot subtract a non-numeric value.");
    }
    auto result = params[0]->asNumber();
    for (size_t i = 1; i < params.size(); ++i) {
        if (!params[i]->isNumber()) {
            throw LispError("Cannot subtract a non-numeric value.");
        }
        result -= params[i]->asNumber();
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr mul(const std::vector<ValuePtr>& params) {
    auto result = 1.0;
    for (const auto& i : params) {
        if (!i->isNumber()) {
            throw LispError("Cannot multiply a non-numeric value.");
        }
        result *= i->asNumber();
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr gt(const std::vector<ValuePtr>& params) {
    if (params.size() != 2) {
        throw LispError("Greater than requires exactly 2 arguments.");
    }
    if (!params[0]->isNumber() || !params[1]->isNumber()) {
        throw LispError("Cannot compare non-numeric values.");
    }
    return std::make_shared<BooleanValue>(params[0]->asNumber() > params[1]->asNumber());
}

ValuePtr lt(const std::vector<ValuePtr>& params) {
    if (params.size() != 2) {
        throw LispError("Less than requires exactly 2 arguments.");
    }
    if (!params[0]->isNumber() || !params[1]->isNumber()) {
        throw LispError("Cannot compare non-numeric values.");
    }
    return std::make_shared<BooleanValue>(params[0]->asNumber() < params[1]->asNumber());
}

ValuePtr print(const std::vector<ValuePtr>& params) {
    if (params.empty()) {
        std::cout << std::endl;
        return std::make_shared<NilValue>();
    }
    std::cout << params[0]->toString();
    for (size_t i = 1; i < params.size(); i++) {
        std::cout << " " << params[i]->toString();
    }
    std::cout << std::endl;
    return std::make_shared<NilValue>();
}

const std::unordered_map<std::string, ValuePtr> Builtins::registry = {
    { "+", std::make_shared<BuiltinProcValue>(&add) },
    { "-", std::make_shared<BuiltinProcValue>(&sub) },
    { "*", std::make_shared<BuiltinProcValue>(&mul) },
    { ">", std::make_shared<BuiltinProcValue>(&gt) },
    { "<", std::make_shared<BuiltinProcValue>(&lt) },
    { "print", std::make_shared<BuiltinProcValue>(&print) }
};

void Builtins::builtins_register(std::unordered_map<std::string, ValuePtr>& symlist) {
    for (const auto& [name, func_ptr] : registry) {
        symlist[name] = func_ptr;
    }
}
