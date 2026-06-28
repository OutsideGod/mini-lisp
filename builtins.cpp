#include "builtins.h"
#include "eval_env.h"

#include <iostream>

ValuePtr add (const std::vector<ValuePtr>& params) {
    auto result = 0.0;
    for (const auto& i : params) {
        if (!i->isNumber()) {
            throw LispError("LispError: " + i->toString() + " is not number");
        }
        result += i->asNumber();
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr minus (const std::vector<ValuePtr>& params) {
    int size = params.size();
    if (size < 1) {
        throw LispError::tooFewOperands(size, 1);
    }
    if (size > 2) {
        throw LispError::tooManyOperands(size, 2);
    }
    if (size == 1) {
        return std::make_shared<NumericValue>(-params[0]->asNumber());
    }
    else {
        if (!params[0]->isNumber()) {
            throw LispError("LispError: " + params[0]->toString() + " is not number");
        }
        if (!params[1]->isNumber()) {
            throw LispError("LispError: " + params[1]->toString() + " is not number");
        }
        return std::make_shared<NumericValue>(params[0]->asNumber() - params[1]->asNumber());
    }
}

ValuePtr multiply (const std::vector<ValuePtr>& params) {
    auto result = 1.0;
    for (const auto& i : params) {
        if (!i->isNumber()) {
            throw LispError("LispError: " + i->toString() + " is not number");
        }
        result *= i->asNumber();
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr divide (const std::vector<ValuePtr>& params) {
    int size = params.size();
    if (size < 1) {
        throw LispError::tooFewOperands(size, 1);
    }
    if (size > 2) {
        throw LispError::tooManyOperands(size, 2);
    }
    if (size == 1) {
        return std::make_shared<NumericValue>(1.0 / params[0]->asNumber());
    }
    else {
        if (!params[0]->isNumber()) {
            throw LispError("LispError: " + params[0]->toString() + " is not number");
        }
        if (!params[1]->isNumber()) {
            throw LispError("LispError: " + params[1]->toString() + " is not number");
        }
    }
    return std::make_shared<NumericValue>(params[0]->asNumber() / params[1]->asNumber());
}

ValuePtr abss (const std::vector<ValuePtr>&params) {
    int size = params.size();
    if (size < 1) {
        throw LispError::tooFewOperands(size, 1);
    }
    if (size > 1) {
        throw LispError::tooManyOperands(size, 1);
    }
    if (!params[0]->isNumber()) {
        throw LispError("LispError: " + params[0]->toString() + " is not number");
    }
    auto b = params[0]->asNumber();
    if (b < 0) {
        b = -b;
    }
    return std::make_shared<NumericValue>(b);
}

ValuePtr expt (const std::vector<ValuePtr>& params) {
    int size = params.size();
    if (size < 2) {
        throw LispError::tooFewOperands(size, 2);
    }
    if (size > 2) {
        throw LispError::tooManyOperands(size, 2);
    }
    for (const auto& i : params) {
        if (!i->isNumber()) {
            throw LispError("LispError: " + i->toString() + " is not number");
        }
    }
    auto a = params[0]->asNumber();
    auto b = params[1]->asNumber();
    auto c = std::pow(a, b);
    return std::make_shared<NumericValue>(c);
}

ValuePtr quotient (const std::vector<ValuePtr>& params) {
    int size = params.size();
    if (size < 2) {
        throw LispError::tooFewOperands(size, 2);
    }
    if (size > 2) {
        throw LispError::tooManyOperands(size, 2);
    }
    for (const auto& i : params) {
        if (!i->isNumber()) {
            throw LispError("LispError: " + i->toString() + " is not number");
        }
    }
    auto a = params[0]->asNumber();
    auto b = params[1]->asNumber();
    double q = std::trunc(a / b);
    return std::make_shared<NumericValue>(q);
}

ValuePtr modulo (const std::vector<ValuePtr>& params) {
    int size = params.size();
    if (size < 2) {
        throw LispError::tooFewOperands(size, 2);
    }
    if (size > 2) {
        throw LispError::tooManyOperands(size, 2);
    }
    for (const auto& i : params) {
        if (!i->isNumber()) {
            throw LispError("LispError: " + i->toString() + " is not number");
        }
    }
    auto a = params[0]->asNumber();
    auto b = params[1]->asNumber();
    auto c = a - b * std::floor(a / b);
    return std::make_shared<NumericValue>(c);
}

ValuePtr remainderr (const std::vector<ValuePtr>& params) {
    int size = params.size();
    if (size < 2) {
        throw LispError::tooFewOperands(size, 2);
    }
    if (size > 2) {
        throw LispError::tooManyOperands(size, 2);
    }
    for (const auto& i : params) {
        if (!i->isNumber()) {
            throw LispError("LispError: " + i->toString() + " is not number");
        }
    }
    auto a = params[0]->asNumber();
    auto b = params[1]->asNumber();
    auto c = a - b * std::trunc(a / b);
    return std::make_shared<NumericValue>(c);
}

ValuePtr eq (const std::vector<ValuePtr>& params) {
    int size = params.size();
    if (size < 2) {
        throw LispError::tooFewOperands(size, 2);
    }
    if (size > 2) {
        throw LispError::tooManyOperands(size, 2);
    }
    auto& a = params[0];
    auto& b = params[1];
    if (typeid(*a) != typeid(*b))
        return std::make_shared<BooleanValue>(false);
    if (typeid(*a) == typeid(BooleanValue) ||
        typeid(*a) == typeid(SymbolValue) ||
        typeid(*a) == typeid(NilValue))
    {
        return std::make_shared<BooleanValue>(a->toString() == b->toString());
    }
    if (a->isNumber()) {
        return std::make_shared<BooleanValue>(a->asNumber() == b->asNumber());
    }
    return std::make_shared<BooleanValue>(a == b);
}

ValuePtr equal (const std::vector<ValuePtr>& params) {
    int size = params.size();
    if (size < 2) {
        throw LispError::tooFewOperands(size, 2);
    }
    if (size > 2) {
        throw LispError::tooManyOperands(size, 2);
    }
    ValuePtr a = params[0], b = params[1];
    if (typeid(*a) != typeid(*b))
        return std::make_shared<BooleanValue>(false);
    if (a->isNumber())
        return std::make_shared<BooleanValue>(a->asNumber() == b->asNumber());
    if (typeid(*a) == typeid(BooleanValue))
        return std::make_shared<BooleanValue>(a->asBool() == b->asBool());
    if (typeid(*a) == typeid(SymbolValue))
        return std::make_shared<BooleanValue>(a->toString() == b->toString());
    if (typeid(*a) == typeid(NilValue))
        return std::make_shared<BooleanValue>(true);
    if (auto sa = std::dynamic_pointer_cast<StringValue>(a))
        return std::make_shared<BooleanValue>(sa->value == std::dynamic_pointer_cast<StringValue>(b)->value);
    if (auto pa = std::dynamic_pointer_cast<PairValue>(a)) {
        auto pb = std::dynamic_pointer_cast<PairValue>(b);
        return equal({ pa->value1, pb->value1 })->asBool()
            ? equal({ pa->value2, pb->value2 })
            : std::make_shared<BooleanValue>(false);
    }
    return std::make_shared<BooleanValue>(a == b);
}

ValuePtr nott (const std::vector<ValuePtr>& params) {
    int size = params.size();
    if (size < 1) {
        throw LispError::tooFewOperands(params.size(), 1);
    }
    if (size > 1) {
        throw LispError::tooManyOperands(params.size(), 1);
    }
    if (params.size() > 1) {
        throw LispError::tooManyOperands(params.size(), 1);
    }
    if (params[0]->asBool() == false) {
        return std::make_shared<BooleanValue>(true);
    }
    else {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr greater (const std::vector<ValuePtr>& params) {
    int size = params.size();
    if (size < 2) {
        throw LispError::tooFewOperands(params.size(), 2);
    }
    if (size > 2) {
        throw LispError::tooManyOperands(params.size(), 2);
    }
    if (!params[0]->isNumber()) {
        throw LispError("LispError: " + params[0]->toString() + " is not number");
    }
    if (!params[1]->isNumber()) {
        throw LispError("LispError: " + params[1]->toString() + " is not number");
    }
    if (params[0]->asNumber() > params[1]->asNumber()) {
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr smaller (const std::vector<ValuePtr>& params) {
    int size = params.size();
    if (size < 2) {
        throw LispError::tooFewOperands(params.size(), 2);
    }
    if (size > 2) {
        throw LispError::tooManyOperands(params.size(), 2);
    }
    if (!params[0]->isNumber()) {
        throw LispError("LispError: " + params[0]->toString() + " is not number");
    }
    if (!params[1]->isNumber()) {
        throw LispError("LispError: " + params[1]->toString() + " is not number");
    }
    if (params[0]->asNumber() < params[1]->asNumber()) {
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr equall (const std::vector<ValuePtr>& params) {
    int size = params.size();
    if (size < 2) {
        throw LispError::tooFewOperands(params.size(), 2);
    }
    if (size > 2) {
        throw LispError::tooManyOperands(params.size(), 2);
    }
    if (!params[0]->isNumber()) {
        throw LispError("LispError: " + params[0]->toString() + " is not number");
    }
    if (!params[1]->isNumber()) {
        throw LispError("LispError: " + params[1]->toString() + " is not number");
    }
    if (params[0]->asNumber() == params[1]->asNumber()) {
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr greatereq (const std::vector<ValuePtr>& params) {
    int size = params.size();
    if (size < 2) {
        throw LispError::tooFewOperands(params.size(), 2);
    }
    if (size > 2) {
        throw LispError::tooManyOperands(params.size(), 2);
    }
    if (!params[0]->isNumber()) {
        throw LispError("LispError: " + params[0]->toString() + " is not number");
    }
    if (!params[1]->isNumber()) {
        throw LispError("LispError: " + params[1]->toString() + " is not number");
    }
    if (params[0]->asNumber() >= params[1]->asNumber()) {
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr smallereq (const std::vector<ValuePtr>& params) {
    int size = params.size();
    if (size < 2) {
        throw LispError::tooFewOperands(params.size(), 2);
    }
    if (size > 2) {
        throw LispError::tooManyOperands(params.size(), 2);
    }
    if (!params[0]->isNumber()) {
        throw LispError("LispError: " + params[0]->toString() + " is not number");
    }
    if (!params[1]->isNumber()) {
        throw LispError("LispError: " + params[1]->toString() + " is not number");
    }
    if (params[0]->asNumber() <= params[1]->asNumber()) {
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr even (const std::vector<ValuePtr>& params) {
    int size = params.size();
    if (size < 1) {
        throw LispError::tooFewOperands(params.size(), 1);
    }
    if (size > 1) {
        throw LispError::tooManyOperands(params.size(), 1);
    }
    auto a = params[0]->asNumber();
    if (std::trunc(a) == a) {
        if (static_cast<int>(a) % 2 == 0) {
            return std::make_shared<BooleanValue>(true);
        }
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr odd(const std::vector<ValuePtr>& params) {
    int size = params.size();
    if (size < 1) {
        throw LispError::tooFewOperands(params.size(), 1);
    }
    if (size > 1) {
        throw LispError::tooManyOperands(params.size(), 1);
    }
    auto a = params[0]->asNumber();
    if (std::trunc(a) == a) {
        if (static_cast<int>(a) % 2 != 0) {
            return std::make_shared<BooleanValue>(true);
        }
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr zero(const std::vector<ValuePtr>& params) {
    int size = params.size();
    if (size < 1) {
        throw LispError::tooFewOperands(params.size(), 1);
    }
    if (size > 1) {
        throw LispError::tooManyOperands(params.size(), 1);
    }
    auto a = params[0]->asNumber();
    if (a == 0) {
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr print(const std::vector<ValuePtr>& params) {
    int size = params.size();
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
    int size = params.size();
    if (size == 0) {
        return std::make_shared<NilValue>();
    }
    if (auto cast = std::dynamic_pointer_cast<StringValue>(params[0])) {
        std::cout << cast->asString();
    }
    else {
        std::cout << params[0]->toString();
    }
    for (int i = 1; i < size; i++) {
        if (auto cast = std::dynamic_pointer_cast<StringValue>(params[i])) {
            std::cout << " " << cast->asString();
        }
        else {
            std::cout << " " << params[i]->toString();
        }
    }
    std::cout << std::endl;
    return std::make_shared<NilValue>();
}

ValuePtr atom(const std::vector<ValuePtr>& params) {
    int size = params.size();
    if (size < 1) {
        throw LispError::tooFewOperands(params.size(), 1);
    }
    if (size > 1) {
        throw LispError::tooManyOperands(params.size(), 1);
    }
    if (typeid(*params[0]) == typeid(BooleanValue) || typeid(*params[0]) == typeid(NumericValue) || typeid(*params[0]) == typeid(StringValue) || typeid(*params[0]) == typeid(SymbolValue) || typeid(*params[0]) == typeid(NilValue)) {
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr booleann(const std::vector<ValuePtr>& params) {
    int size = params.size();
    if (size < 1) {
        throw LispError::tooFewOperands(params.size(), 1);
    }
    if (size > 1) {
        throw LispError::tooManyOperands(params.size(), 1);
    }
    if (typeid(*params[0]) == typeid(BooleanValue)) {
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr integer(const std::vector<ValuePtr>& params) {
    int size = params.size();
    if (size < 1) {
        throw LispError::tooFewOperands(params.size(), 1);
    }
    if (size > 1) {
        throw LispError::tooManyOperands(params.size(), 1);
    }
    if (auto cast = std::dynamic_pointer_cast<NumericValue>(params[0])) {
        if (std::trunc(cast->asNumber()) == cast->asNumber()) {
            return std::make_shared<BooleanValue>(true);
        }
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr list(const std::vector<ValuePtr>& params) {
    int size = params.size();
    if (size < 1) {
        throw LispError::tooFewOperands(params.size(), 1);
    }
    if (size > 1) {
        throw LispError::tooManyOperands(params.size(), 1);
    }
    ValuePtr val = params[0];
    if (typeid(*val) == typeid(NilValue)) {
        return std::make_shared<BooleanValue>(true);
    }
    auto pair = std::dynamic_pointer_cast<PairValue>(val);
    if (!pair) {
        return std::make_shared<BooleanValue>(false);
    }
    ValuePtr curr = val;
    while (true) {
        auto p = std::dynamic_pointer_cast<PairValue>(curr);
        if (!p) break;
        curr = p->value2;
        if (!curr) return std::make_shared<BooleanValue>(false);
    }
    return std::make_shared<BooleanValue>(typeid(*curr) == typeid(NilValue));
}

ValuePtr number(const std::vector<ValuePtr>& params) {
    int size = params.size();
    if (size < 1) {
        throw LispError::tooFewOperands(params.size(), 1);
    }
    if (size > 1) {
        throw LispError::tooManyOperands(params.size(), 1);
    }
    if (typeid(*params[0]) == typeid(NumericValue)) {
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr null(const std::vector<ValuePtr>& params) {
    int size = params.size();
    if (size < 1) {
        throw LispError::tooFewOperands(params.size(), 1);
    }
    if (size > 1) {
        throw LispError::tooManyOperands(params.size(), 1);
    }
    if (typeid(*params[0]) == typeid(NilValue)) {
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr pair(const std::vector<ValuePtr>& params) {
    int size = params.size();
    if (size < 1) {
        throw LispError::tooFewOperands(params.size(), 1);
    }
    if (size > 1) {
        throw LispError::tooManyOperands(params.size(), 1);
    }
    if (typeid(*params[0]) == typeid(PairValue)) {
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr procedure(const std::vector<ValuePtr>&params) {
    int size = params.size();
    if (size < 1) {
        throw LispError::tooFewOperands(params.size(), 1);
    }
    if (size > 1) {
        throw LispError::tooManyOperands(params.size(), 1);
    }
    if (typeid(*params[0]) == typeid(BuiltinProcValue) || typeid(*params[0]) == typeid(LambdaValue)) {
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr string(const std::vector<ValuePtr>& params) {
    int size = params.size();
    if (size < 1) {
        throw LispError::tooFewOperands(params.size(), 1);
    }
    if (size > 1) {
        throw LispError::tooManyOperands(params.size(), 1);
    }
    if (typeid(*params[0]) == typeid(StringValue)) {
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr symbol(const std::vector<ValuePtr>& params) {
    int size = params.size();
    if (size < 1) {
        throw LispError::tooFewOperands(params.size(), 1);
    }
    if (size > 1) {
        throw LispError::tooManyOperands(params.size(), 1);
    }
    if (typeid(*params[0]) == typeid(SymbolValue)) {
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

const std::unordered_map<std::string, ValuePtr> Builtins::registry = {
    {"+", std::make_shared<BuiltinProcValue>(&add)},
    {"-", std::make_shared<BuiltinProcValue>(&minus)},
    {"*", std::make_shared<BuiltinProcValue>(&multiply)},
    {"/", std::make_shared<BuiltinProcValue>(&divide)},
    {"abs", std::make_shared<BuiltinProcValue>(&abss)},
    {"expt", std::make_shared<BuiltinProcValue>(&expt)},
    {"quotient", std::make_shared<BuiltinProcValue>(&quotient)},
    {"modulo", std::make_shared<BuiltinProcValue>(&modulo)},
    {"remainder", std::make_shared<BuiltinProcValue>(&remainderr)},
    {"eq?", std::make_shared<BuiltinProcValue>(&eq)},
    {"equal?", std::make_shared<BuiltinProcValue>(&equal)},
    {"not", std::make_shared<BuiltinProcValue>(&nott)},
    {"=", std::make_shared<BuiltinProcValue>(&equall)},
    {">", std::make_shared<BuiltinProcValue>(&greater)},
    {"<", std::make_shared<BuiltinProcValue>(&smaller)},
    {">=", std::make_shared<BuiltinProcValue>(&greatereq)},
    {"<=", std::make_shared<BuiltinProcValue>(&smallereq)},
    {"even?", std::make_shared<BuiltinProcValue>(&even)},
    {"odd?", std::make_shared<BuiltinProcValue>(&odd)},
    {"zero?", std::make_shared<BuiltinProcValue>(&zero)},
    {"print", std::make_shared<BuiltinProcValue>(&print)},
    {"display", std::make_shared<BuiltinProcValue>(&display)},
    {"atom?", std::make_shared<BuiltinProcValue>(&atom)},
    {"boolean?", std::make_shared<BuiltinProcValue>(&booleann)},
    {"integer?", std::make_shared<BuiltinProcValue>(&integer)},
    {"list?", std::make_shared<BuiltinProcValue>(&list)},
    {"number?", std::make_shared<BuiltinProcValue>(&number)},
    {"null?", std::make_shared<BuiltinProcValue>(&null)},
    {"pair?", std::make_shared<BuiltinProcValue>(&pair)},
    {"procedure?", std::make_shared<BuiltinProcValue>(&procedure)},
    {"string?", std::make_shared<BuiltinProcValue>(&string)},
    {"symbol?", std::make_shared<BuiltinProcValue>(&symbol)}
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
