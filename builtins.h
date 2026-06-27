#ifndef BUILTINS_H
#define BUILTINS_H
#include <unordered_map>

#include "value.h"

class EvalEnv;

class Builtins {
    static const std::unordered_map<std::string, ValuePtr> registry;
public:
    static const std::unordered_map<std::string, ValuePtr>* get_registry() {
        return &registry;
    }
    static BuiltinFuncType makeEval(std::shared_ptr<EvalEnv>);
    static BuiltinFuncType makeApply(std::shared_ptr<EvalEnv>);
};

#endif
