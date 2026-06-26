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


namespace Forms {
    const std::unordered_map<std::string, SpecialFormType> SPECIAL_FORMS{
    {"lambda", lambdaForm},
    {"define", defineForm}, 
    {"quote", quoteForm},
    {"if", ifForm},
    {"and", andForm},
    {"or", orForm}
    };
}
