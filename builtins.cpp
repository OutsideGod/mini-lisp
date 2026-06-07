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

ValuePtr print(const std::vector<ValuePtr>& params) {
    std::cout << params[0]->toString();
    for (int i = 1; i < params.size(); i++) {
        std::cout << " " << params[i]->toString();
    }
    std::cout << std::endl;
    return std::make_shared<NilValue>();
}

const std::unordered_map<std::string, ValuePtr> Builtins::registry = {
    { "+", std::make_shared<BuiltinProcValue>(&add)},
    { "print", std::make_shared<BuiltinProcValue>(&print) }
};

void Builtins::builtins_register(std::unordered_map<std::string, ValuePtr> &symlist) {
    for (const auto& [name, func_ptr] : registry) {
        symlist[name] = func_ptr;
    }
}
