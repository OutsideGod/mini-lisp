#ifndef EVALENV_H
#define EVALENV_H

#include "value.h"
#include "builtins.h"
#include "forms.h"
#include "error.h"

#include <unordered_map>
#include <algorithm>
#include <iterator>

class EvalEnv {
public:
    EvalEnv();
    EvalEnv(EvalEnv* parent);
    EvalEnv* parent;
    static thread_local EvalEnv* current_env;
    std::unordered_map<std::string, ValuePtr> symlist;
    ValuePtr eval(ValuePtr expr);
    std::vector<ValuePtr> evalList(std::vector<ValuePtr>);
    ValuePtr apply(ValuePtr, std::vector<ValuePtr>);
};

#endif
