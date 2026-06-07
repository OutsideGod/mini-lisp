#include "eval_env.h"

using namespace std::literals;

EvalEnv::EvalEnv() {
    Builtins::builtins_register(symlist);
}

std::vector<ValuePtr> EvalEnv::evalList(std::vector<ValuePtr> v) {
    std::vector<ValuePtr> result;
    std::ranges::transform(v,std::back_inserter(result),[this](ValuePtr v) { return this->eval(v); });
    return result;
}

ValuePtr EvalEnv::apply(ValuePtr proc, std::vector<ValuePtr> args) {
    if (typeid(*proc) == typeid(BuiltinProcValue)) {
        return proc->Activatefunc(args);
    }
    else {
        throw LispError("Unimplemented");
    }
}

ValuePtr EvalEnv::eval(ValuePtr expr) {
    if (!expr || typeid(*expr) == typeid(NilValue)) {
        throw LispError("Evaluating nil is prohibited.");
    }
    if (typeid(*expr) == typeid(BooleanValue) || typeid(*expr) == typeid(StringValue) || typeid(*expr) == typeid(NumericValue)) {
        return expr;
    }
    if (auto name = expr->asSymbol()) {
        auto iter = symlist.find(*name);
        if (iter != symlist.end()) {
            return iter->second;
        }
        else {
            throw LispError("Variable " + *name + " not defined.");
        }
    }
    if (auto transexpr = std::dynamic_pointer_cast<PairValue>(expr)) {
        std::vector<ValuePtr> v = transexpr->toVector();
        if (auto name = v[0]->asSymbol()) {
            auto it = Forms::specialforms.find(*name);
            const std::vector<ValuePtr> pure_params(v.begin() + 1, v.end());
            if (it != Forms::specialforms.end()) {
                return it->second(pure_params, *this);
            }
        }
        const std::vector<ValuePtr> pure_params(v.begin() + 1, v.end());
        ValuePtr proc = this->eval(v[0]);
        std::vector<ValuePtr> args = EvalEnv::evalList(pure_params);
        return this->apply(proc, args);
    }
    else {
        throw LispError("Unimplemented");
    }
}
