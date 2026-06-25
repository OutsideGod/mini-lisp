#ifndef FORMS_H
#define FORMS_H

#include "value.h"

#include <unordered_map>
#include <string>
#include <vector>

class EvalEnv;
using SpecialFormType = ValuePtr(const std::vector<ValuePtr>&, EvalEnv&);

class Forms {
public:
    static const std::unordered_map<std::string, SpecialFormType*> specialforms;
};

#endif
