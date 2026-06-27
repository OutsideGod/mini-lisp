#include "builtins.h"
#include "eval_env.h"

#include <iostream>

ValuePtr add(const std::vector<ValuePtr>& params) {
    auto result = 0.0;
    for (const auto& i : params) {
        if (!i->isNumber()) {
            throw LispError("LispError: " + i->toString() + " is not number");
        }
        result += i->asNumber();
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr minus(const std::vector<ValuePtr>& params) {
    int size = params.size();
    if (size < 1) {
        throw LispError::tooFewOperands(size, 1);
    }
    if (size > 2) {
        throw LispError::tooManyOperands(size, 2);
    }
    if (!params[0]->isNumber()) {
        throw LispError("LispError: " + params[0]->toString() + " is not number");
    }
    if (!params[1]->isNumber()) {
        throw LispError("LispError: " + params[1]->toString() + " is not number");
    }
    return std::make_shared<NumericValue>(params[0]->asNumber() - params[1]->asNumber());
}

ValuePtr multiply(const std::vector<ValuePtr>& params) {
    auto result = 1.0;
    for (const auto& i : params) {
        if (!i->isNumber()) {
            throw LispError("LispError: " + i->toString() + " is not number");
        }
        result *= i->asNumber();
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr divide(const std::vector<ValuePtr>& params) {
    int size = params.size();
    if (size < 1) {
        throw LispError::tooFewOperands(size, 1);
    }
    if (size > 2) {
        throw LispError::tooManyOperands(size, 2);
    }
    if (!params[0]->isNumber()) {
        throw LispError("LispError: " + params[0]->toString() + " is not number");
    }
    if (!params[1]->isNumber()) {
        throw LispError("LispError: " + params[1]->toString() + " is not number");
    }
    return std::make_shared<NumericValue>(params[0]->asNumber() / params[1]->asNumber());
}

ValuePtr greater(const std::vector<ValuePtr>& params) {
    auto size = params.size();
    if (size < 2){
        throw LispError::tooFewOperands(size, 2);
    }
    if (!params[0]->isNumber()) {
        throw LispError("LispError: " + params[0]->toString() + " is not number");
    }
    for (int i = 1; i < size; i++) {
        if (!params[i]->isNumber()) {
            throw LispError("LispError: " + params[i]->toString() + " is not number");
        }
        if (params[i-1]->asNumber() <= params[i]->asNumber()) {
            return std::make_shared<BooleanValue>(false);
        }
    }
    return std::make_shared<BooleanValue>(true);
}

ValuePtr smaller(const std::vector<ValuePtr>& params) {
    auto size = params.size();
    if (size < 2) {
        throw LispError::tooFewOperands(size, 2);
    }
    if (!params[0]->isNumber()) {
        throw LispError("LispError: " + params[0]->toString() + " is not number");
    }
    for (int i = 1; i < size; i++) {
        if (!params[i]->isNumber()) {
            throw LispError("LispError: " + params[i]->toString() + " is not number");
        }
        if (params[i - 1]->asNumber() >= params[i]->asNumber()) {
            return std::make_shared<BooleanValue>(false);
        }
    }
    return std::make_shared<BooleanValue>(true);
}

ValuePtr print(const std::vector<ValuePtr>& params) {
    auto size = params.size();
    if (size == 0) {
        return std::make_shared<NilValue>();
    }
    std::cout << params[0]->toString();
    for (int i = 1; i < size; i++) {
        std::cout << " " << params[i]->toString();
    }
    std::cout << std::endl;
    return std::make_shared<NilValue>();
}

ValuePtr display(const std::vector<ValuePtr>& params) {
    auto size = params.size();
    if (size == 0) {
        return std::make_shared<NilValue>();
    }
    if (typeid(*params[0]) == typeid(StringValue)) {
        std::cout << params[0]->toString().substr(1, params[0]->toString().size() - 2);
    }
    else {
        std::cout << params[0]->toString();
    }
    for (int i = 1; i < size; i++) {
        if (typeid(*params[i]) == typeid(StringValue)) {
            std::cout << " " << params[i]->toString().substr(1, params[i]->toString().size() - 2);
        }
        else {
            std::cout << " " << params[i]->toString();
        }
    }
    std::cout << std::endl;
    return std::make_shared<NilValue>();
}

const std::unordered_map<std::string, ValuePtr> Builtins::registry = {
    {"+", std::make_shared<BuiltinProcValue>(&add)},
    {"-", std::make_shared<BuiltinProcValue>(&minus)},
    {"*", std::make_shared<BuiltinProcValue>(&multiply)},
    {"/", std::make_shared<BuiltinProcValue>(&divide)},
    {">", std::make_shared<BuiltinProcValue>(&greater)},
    {"<", std::make_shared<BuiltinProcValue>(&smaller)},
    {"print", std::make_shared<BuiltinProcValue>(&print)},
    {"display", std::make_shared<BuiltinProcValue>(&display)}
};

BuiltinFuncType Builtins::makeEval(std::shared_ptr<EvalEnv> env) {
    return [env](const std::vector<ValuePtr>& params) -> ValuePtr {
        auto size = params.size();
        if (size < 1) {
            throw LispError::tooFewOperands(size, 1);
        }
        if (size > 1) {
            throw LispError::tooManyOperands(size, 1);
        }
        return env->eval(params[0]);
        };
}

BuiltinFuncType Builtins::makeApply(std::shared_ptr<EvalEnv> env) {
    return [env](const std::vector<ValuePtr>& params) -> ValuePtr {
        auto size = params.size();
        if (size < 2) {
            throw LispError::tooFewOperands(size, 2);
        }
        if (size > 2) {
            throw LispError::tooManyOperands(size, 2);
        }
        if (typeid(*params[1]) != typeid(PairValue) && typeid(*params[1]) != typeid(NilValue)) {
            throw LispError("LispError: Malformed list: expected pair or nil, got " + params[1]->toString());
        }
        ValuePtr proc = env->eval(params[0]);
        std::vector<ValuePtr> listArgs = params[1]->toVector();
        return env->apply(proc, listArgs);
        };
}
