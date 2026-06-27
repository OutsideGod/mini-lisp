#ifndef EVALENV_H
#define EVALENV_H

#include <algorithm>
#include <iterator>
#include <unordered_map>

#include "./value.h"
#include "forms.h"

class EvalEnv : public std::enable_shared_from_this<EvalEnv> {
private:
    std::unordered_map<std::string, ValuePtr> symlist;
    std::shared_ptr<EvalEnv> parent;

    void env_register();
public:
    EvalEnv(std::shared_ptr<EvalEnv> a = nullptr);
    ValuePtr eval(ValuePtr expr);
    std::vector<ValuePtr> evalList(std::vector<ValuePtr>);
    ValuePtr apply(ValuePtr, std::vector<ValuePtr>);
    ValuePtr lookupBinding(std::string);
    void defineBinding(std::string, ValuePtr);
    std::shared_ptr<EvalEnv> createChild(const std::vector<std::string>& params, const std::vector<ValuePtr>& args);
    static std::shared_ptr<EvalEnv> createGlobal();
};

#endif
