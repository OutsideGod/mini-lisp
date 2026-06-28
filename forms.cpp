#include "forms.h"
#include "eval_env.h"
#include "token.h"

ValuePtr lambdaForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    int size = args.size();
    if (size < 2) {
        throw LispError::tooFewOperands(size, 2);
    }
    if (auto transexpr = std::dynamic_pointer_cast<NilValue>(args[0])) {
        std::vector<std::string> result_params = {};
        std::vector<ValuePtr> body;
        for (int i = 1; i < args.size(); i++) {
            body.push_back(args[i]);
        }
        return std::make_shared<LambdaValue>(result_params, body, env.shared_from_this());
    }
    else if (auto _ = args[0]->asSymbol()) {
        throw LispError("Unimplemented");
    }
    else if (auto transexpr = std::dynamic_pointer_cast<PairValue>(args[0])) {
        std::vector<ValuePtr> initial_params = args[0]->toVector();
        std::vector<std::string> result_params;
        std::vector<ValuePtr> body;
        for (auto element : initial_params) {
            if (auto name = element->asSymbol()) {
                result_params.push_back(*name);
            }
            else {
                throw LispError::unexpectedElement("symbol", "Lambda parameter", element);
            }
        }
        for (int i = 1; i < args.size(); i++) {
            body.push_back(args[i]);
        }
        return std::make_shared<LambdaValue>(result_params, body, env.shared_from_this());
    }
    else {
        throw LispError::unexpectedElement("symbol or list", "Lambda expression", args[0]);
    }
}

ValuePtr defineForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    int size = args.size();
    if (size < 2) {
        throw LispError::tooFewOperands(0, 2);
    }
    if (auto name = args[0]->asSymbol()) {
        env.defineBinding(*name, args[1]);
    }
    else if (auto params = std::dynamic_pointer_cast<PairValue>(args[0])) {
        auto name = params->value1->asSymbol();
        if (!name) {
            throw LispError("LispError: In lambda definition, " + params->value1->toString() + " is not a symbol name");
        }
        std::vector<ValuePtr> lambda_args;
        lambda_args.push_back(params->getCdr());
        for (int i = 1; i < args.size(); i++) {
            lambda_args.push_back(args[i]);
        }
        env.defineBinding(*name, lambdaForm(lambda_args, env));
    }
    else {
        throw LispError("Unimplemented");
    }
    return std::make_shared<NilValue>();
}

ValuePtr quoteForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    int size = args.size();
    if (size == 1) {
        return args[0];
    }
    else if (size == 0) {
        throw LispError::tooFewOperands(size, 1);
    }
    else {
        throw LispError::tooManyOperands(size, 1);
    }
}

ValuePtr ifForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    int size = args.size();
    if (size < 2) {
        throw LispError::tooFewOperands(size, 2);
    }
    else if (size > 3) {
        throw LispError::tooManyOperands(size, 3);
    }
    else {
        if (env.eval(args[0])->asBool() == false) {
            if (size == 2) {
                return std::make_shared<NilValue>();
            }
            else {
                return env.eval(args[2]);
            }
        }
        else {
            return env.eval(args[1]);
        }
    }
}

ValuePtr andForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    int size = args.size();
    if (size == 0) {
        return std::make_shared<BooleanValue>(true);
    }
    else {
        for (auto element : args) {
            if (env.eval(element)->asBool() == false) {
                return env.eval(element);
            }
        }
    }
    return env.eval(args.back());
}

ValuePtr orForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    int size = args.size();
    for (auto element : args) {
        if (env.eval(element)->asBool() == true) {
            return env.eval(element);
        }
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr condForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    int size = args.size();
    ValuePtr result = std::make_shared<NilValue>();
    if (size == 0) {
        return result;
    }
    for (int i = 0; i < args.size(); i++) {
        if (auto cast = std::dynamic_pointer_cast<PairValue>(args[i])) {
            auto head = cast->value1;
            auto body = cast->getCdr();
            bool isElse = false;
            if (auto sym = std::dynamic_pointer_cast<SymbolValue>(head)) {
                if (sym->value == "else") isElse = true;
            }
            if (typeid(body) == typeid(NilValue)) {
                if (isElse) {
                    if (i != args.size() - 1) {
                        throw LispError("LispError: else clause must be the last one");
                    }
                    else {
                        throw LispError("LispError: Missing expressions in `'else' clause");
                    }
                }
                result = env.eval(head);
                if (result->asBool() == true) {
                    return result;
                }
            }
            else {
                std::vector<ValuePtr> body_vector = body->toVector();
                result = isElse ? std::make_shared<BooleanValue>(true) : env.eval(head);
                if (result->asBool() == true) {
                    if (isElse && i != args.size() - 1) {
                        throw LispError("LispError: else clause must be the last one");
                    }
                    for (auto sub_element : body_vector) {
                        result = env.eval(sub_element);
                    }
                    return result;
                }
            }
        }
        else {
            throw LispError::unexpectedElement("pair", "cond clause", args[i]);
        }
    }
    return result;
}


ValuePtr beginForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    ValuePtr result = std::make_shared<NilValue>();
    for (auto element : args) {
        result = env.eval(element);
    }
    return result;
}

ValuePtr letForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() < 2)
        throw LispError::tooFewOperands(args.size(), 2);

    ValuePtr bindings = args[0];

    std::vector<std::string> vars;
    std::vector<ValuePtr> inits;
    ValuePtr cur = bindings;
    while (auto pair = std::dynamic_pointer_cast<PairValue>(cur)) {
        auto bind = std::dynamic_pointer_cast<PairValue>(pair->value1);
        if (!bind) throw LispError::unexpectedElement("pair or nil", "arguments", pair->value1);
        auto varSym = std::dynamic_pointer_cast<SymbolValue>(bind->value1);
        if (!varSym) throw LispError::unexpectedElement("let binding name", "arguments", bind->value1);

        std::vector<ValuePtr> parts = bind->toVector();
        if (parts.size() < 2) {
            throw LispError::tooFewOperands(parts.size(), 2);
        }
        if (parts.size() > 2) {
            throw LispError::tooManyOperands(parts.size(), 2);
        }
        vars.push_back(varSym->value);
        inits.push_back(parts[1]);
        cur = pair->value2;
    }
    if (typeid(*cur) != typeid(NilValue))
        throw LispError::unexpectedElement("pair or nil", "arguments", cur);

    ValuePtr paramsList = std::make_shared<NilValue>();
    for (int i = vars.size() - 1; i >= 0; --i)
        paramsList = std::make_shared<PairValue>(
            std::make_shared<SymbolValue>(vars[i]), paramsList);

    std::vector<ValuePtr> lambdaArgs;
    lambdaArgs.push_back(paramsList);
    for (int i = 1; i < args.size(); ++i)
        lambdaArgs.push_back(args[i]);

    ValuePtr lambdaVal = lambdaForm(lambdaArgs, env);
    auto proc = std::dynamic_pointer_cast<LambdaValue>(lambdaVal);
    std::vector<ValuePtr> initVals;
    for (auto& init : inits)
        initVals.push_back(env.eval(init));

    return proc->apply(initVals);
}

ValuePtr quasiquoteForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 1)
        throw LispError::tooManyOperands(args.size(), 1);

    ValuePtr tmpl = args[0];
    auto pair = std::dynamic_pointer_cast<PairValue>(tmpl);
    if (!pair) return tmpl;

    std::vector<ValuePtr> elems = tmpl->toVector();
    std::vector<ValuePtr> resultElems;

    for (auto& e : elems) {
        auto ePair = std::dynamic_pointer_cast<PairValue>(e);
        if (ePair) {
            auto sym = std::dynamic_pointer_cast<SymbolValue>(ePair->value1);
            if (sym && sym->value == "unquote") {
                auto inner = std::dynamic_pointer_cast<PairValue>(ePair->value2);
                if (!inner)
                    throw LispError("unquote: missing argument");
                resultElems.push_back(env.eval(inner->value1));
                continue;
            }
        }
        resultElems.push_back(e);
    }
    ValuePtr result = std::make_shared<NilValue>();
    for (int i = resultElems.size() - 1; i >= 0; --i)
        result = std::make_shared<PairValue>(resultElems[i], result);
    return result;
}

namespace Forms {
    const std::unordered_map<std::string, SpecialFormType> SPECIAL_FORMS{
    {"lambda", lambdaForm},
    {"define", defineForm}, 
    {"quote", quoteForm},
    {"if", ifForm},
    {"and", andForm},
    {"or", orForm},
    {"cond", condForm},
    {"begin", beginForm},
    {"let", letForm},
    {"quasiquote", quasiquoteForm}
    };
}
