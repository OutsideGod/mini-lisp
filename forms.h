#ifndef FORMS_H
#define FORMS_H

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "value.h"

class EvalEnv;

using SpecialFormType = std::function<ValuePtr(const std::vector<ValuePtr>&, EvalEnv&)>;

namespace Forms {
    extern const std::unordered_map<std::string, SpecialFormType> SPECIAL_FORMS;
};

#endif
