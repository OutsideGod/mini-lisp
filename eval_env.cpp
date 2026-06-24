#include "eval_env.h"

using namespace std::literals;

thread_local EvalEnv* EvalEnv::current_env = nullptr;

EvalEnv::EvalEnv() : parent(nullptr) {
    Builtins::builtins_register(symlist);
}

EvalEnv::EvalEnv(EvalEnv* parent) : parent(parent) {}

std::vector<ValuePtr> EvalEnv::evalList(std::vector<ValuePtr> v) {
    std::vector<ValuePtr> result;
    std::ranges::transform(v, std::back_inserter(result), [this](ValuePtr v) { return this->eval(v); });
    return result;
}

ValuePtr EvalEnv::apply(ValuePtr proc, std::vector<ValuePtr> args) {
    if (typeid(*proc) == typeid(BuiltinProcValue)) {
        return proc->Activatefunc(args);
    }
    else if (auto lambda = std::dynamic_pointer_cast<LambdaValue>(proc)) {
        EvalEnv* lambda_env = lambda->getEnv();
        EvalEnv* child_env = new EvalEnv(lambda_env ? lambda_env : this);

        auto params = lambda->getParams();
        if (params.size() != args.size()) {
            throw LispError("Argument count mismatch.");
        }

        for (size_t i = 0; i < params.size(); ++i) {
            child_env->symlist[params[i]] = args[i];
        }

        auto body = lambda->getBody();
        if (body.empty()) {
            return std::make_shared<NilValue>();
        }

        ValuePtr res = nullptr;
        for (const auto& expr : body) {
            res = child_env->eval(expr);
        }
        return res;
    }
    else {
        throw LispError("Unimplemented");
    }
}

ValuePtr EvalEnv::eval(ValuePtr expr) {
    struct EnvGuard {
        EvalEnv* old_env;
        EnvGuard(EvalEnv* new_env) {
            old_env = EvalEnv::current_env;
            EvalEnv::current_env = new_env;
        }
        ~EnvGuard() {
            EvalEnv::current_env = old_env;
        }
    } guard(this);

    if (!expr || typeid(*expr) == typeid(NilValue)) {
        throw LispError("Evaluating nil is prohibited.");
    }
    if (typeid(*expr) == typeid(BooleanValue) || typeid(*expr) == typeid(StringValue) || typeid(*expr) == typeid(NumericValue)) {
        return expr;
    }
    if (auto name = expr->asSymbol()) {
        const EvalEnv* curr = this;
        while (curr != nullptr) {
            auto iter = curr->symlist.find(*name);
            if (iter != curr->symlist.end()) {
                return iter->second;
            }
            curr = curr->parent;
        }
        throw LispError("Variable " + *name + " not defined.");
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

LambdaValue::LambdaValue(std::vector<std::string> p, std::vector<ValuePtr> b)
    : params(p), body(b), env(EvalEnv::current_env) {
}

std::string LambdaValue::toString() {
    return "#<procedure>";
}
