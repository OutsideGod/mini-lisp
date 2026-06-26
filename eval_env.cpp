#include "./eval_env.h"

#include "./error.h"

using namespace std::literals;

EvalEnv::EvalEnv(std::shared_ptr<EvalEnv> a) {
    parent = a;
    Builtins::builtins_register(symlist);
}

std::vector<ValuePtr> EvalEnv::evalList(std::vector<ValuePtr> v) {
    std::vector<ValuePtr> result;
    std::ranges::transform(v, std::back_inserter(result),
                           [this](ValuePtr v) { return this->eval(v); });
    return result;
}

ValuePtr EvalEnv::apply(ValuePtr proc, std::vector<ValuePtr> args) {
    if (typeid(*proc) == typeid(BuiltinProcValue)) {
        return proc->Activatefunc(args);
    } 
    else if (auto lambda = std::dynamic_pointer_cast<LambdaValue>(proc) ) {
        return lambda->apply(args);
    }
    else {
        throw LispError("Unimplemented");
    }
}

ValuePtr EvalEnv::lookupBinding(std::string name) {
    auto iter = symlist.find(name);
    if (iter != symlist.end()) {
        return iter->second;
    }
    else {
        if (parent == nullptr) {
            throw LispError("LispError: Unbound variable " + name);
        }
        else {
            return parent->lookupBinding(name);
        }
    }
}

void EvalEnv::defineBinding(std::string a, ValuePtr b) {
    symlist[a] = eval(b);
}

std::shared_ptr<EvalEnv> EvalEnv::createChild(const std::vector<std::string>& params, const std::vector<ValuePtr>& args) {
    auto child = std::make_shared<EvalEnv>(shared_from_this());
    if (params.size() != args.size()) {
        throw LispError("LispError: Procedure expected " + std::to_string(params.size()) + " parameters, got " + std::to_string(args.size()));
    }
    for (int i = 0; i < params.size(); i++) {
        child->defineBinding(params[i], args[i]);
    }
    return child;
}

ValuePtr EvalEnv::eval(ValuePtr expr) {
    if (!expr) {
        throw EOFError("EOFError: Unexpected end of file");
    }
    if (typeid(*expr) == typeid(NilValue)) {
        throw LispError("LispError: Shouldn't evaluate empty list");
    }
    if (typeid(*expr) == typeid(BooleanValue) ||
        typeid(*expr) == typeid(StringValue) ||
        typeid(*expr) == typeid(NumericValue) ||
        typeid(*expr) == typeid(LambdaValue) ||
        typeid(*expr) == typeid(BuiltinProcValue)) {
        return expr;
    }
    if (auto name = expr->asSymbol()) {
        return lookupBinding(*name);
    }
    if (auto transexpr = std::dynamic_pointer_cast<PairValue>(expr)) {
        if (auto name = transexpr->value1->asSymbol()) {
            auto iter = Forms::SPECIAL_FORMS.find(*name);
            if (iter != Forms::SPECIAL_FORMS.end()) {
                return iter->second(transexpr->getCdr()->toVector(), *this);
            }
        } 
        ValuePtr proc = this->eval(transexpr->value1);
        std::vector<ValuePtr> args = EvalEnv::evalList(transexpr->getCdr()->toVector());
        return this->apply(proc, args);
    }

    else {
        throw LispError("Unimplemented");
    }
}
