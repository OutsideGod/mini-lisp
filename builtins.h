#ifndef BUILTINS_H
#define BUILTINS_H
#include <unordered_map>

#include "value.h"

class Builtins {
    static const std::unordered_map<std::string, ValuePtr> registry;
public:
    static void builtins_register(std::unordered_map<std::string, ValuePtr> &);
};


#endif
