#include"forms.h"
#include "eval_env.h"


ValuePtr defineForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() >= 2) {
        if (auto name = args[0]->asSymbol()) {
            env.symlist[*name] = env.eval(args[1]);
            return std::make_shared<NilValue>();
        }
        else if (auto pair = std::dynamic_pointer_cast<PairValue>(args[0])) {
            auto lst = pair->toVector();
            if (lst.empty()) {
                throw LispError("Malformed define: empty function signature.");
            }
            if (auto func_name_opt = lst[0]->asSymbol()) {
                std::string func_name = *func_name_opt;
                std::vector<std::string> params;
                for (size_t i = 1; i < lst.size(); ++i) {
                    if (auto param_name = lst[i]->asSymbol()) {
                        params.push_back(*param_name);
                    }
                    else {
                        throw LispError("Function parameters must be symbols.");
                    }
                }
                std::vector<ValuePtr> body(args.begin() + 1, args.end());
                env.symlist[func_name] = std::make_shared<LambdaValue>(params, body);
                return std::make_shared<NilValue>();
            }
            else {
                throw LispError("Function name must be a symbol.");
            }
        }
        else {
            throw LispError("Malformed define: first argument must be a symbol or a list.");
        }
    }
    else {
        throw LispError("Too few operands: " + std::to_string(args.size()) + " < 2");
    }
}
ValuePtr quoteForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() == 1) {
        return args[0];
    }
    else {
        throw LispError("SyntaxError");
    }
}

ValuePtr ifForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() == 3) {
        ValuePtr cond = env.eval(args[0]);
        if (cond->toString() == "#f" && typeid(*cond) == typeid(BooleanValue)) {
            return env.eval(args[2]);
        }
        else {
            return env.eval(args[1]);
        }
    }
    else {
        throw LispError("SyntaxError");
    }
}

ValuePtr andForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.empty()) {
        return env.eval(std::make_shared<BooleanValue>(true));
    }
    else {
        ValuePtr result;
        for (ValuePtr element : args) {
            ValuePtr cond = env.eval(element);
            if (cond->toString() == "#f" && typeid(*cond) == typeid(BooleanValue)) {
                return cond;
            }
            result = cond;
        }
        return result;
    }
}

ValuePtr orForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.empty()) {
        return env.eval(std::make_shared<BooleanValue>(false));
    }
    else {
        ValuePtr result;
        for (ValuePtr element : args) {
            ValuePtr cond = env.eval(element);
            if (cond->toString() != "#f" || typeid(*cond) != typeid(BooleanValue)) {
                return cond;
            }
            result = cond;
        }
        return result;
    }
}

ValuePtr lambdaForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() >= 2) {
        std::vector<std::string> params;
        if (auto pair = std::dynamic_pointer_cast<PairValue>(args[0])) {
            for (const auto& v : pair->toVector()) {
                if (auto name = v->asSymbol()) {
                    params.push_back(*name);
                }
                else {
                    throw LispError("Expect symbol in Lambda parameter");
                }
            }
        }
        else if (std::dynamic_pointer_cast<NilValue>(args[0])) {
        }
        else {
            throw LispError("Lambda parameter list must be a list.");
        }
        std::vector<ValuePtr> body(args.begin() + 1, args.end());
        return std::make_shared<LambdaValue>(params, body);
    }
    else {
        throw LispError("Too few operands: " + std::to_string(args.size()) + " < 2");
    }
}

const std::unordered_map<std::string, SpecialFormType*> Forms::specialforms = {
    { "define", defineForm },
    { "quote", quoteForm },
    { "if", ifForm },
    { "and", andForm },
    { "or", orForm },
    { "lambda", lambdaForm }
};
