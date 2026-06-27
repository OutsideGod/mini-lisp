#ifndef ERROR_H
#define ERROR_H

#include <stdexcept>

class SyntaxError : public std::runtime_error {
public:
    using runtime_error::runtime_error;
};

class LispError : public std::runtime_error {
public:
    using runtime_error::runtime_error;
    static LispError tooManyOperands(int current, int goal) {
        return LispError("LispError: Too many operands: " + std::to_string(current) + " > " + std::to_string(goal));
    }
    static LispError tooFewOperands(int current, int goal) {
        return LispError("LispError: Too few operands: " + std::to_string(current) + " < " + std::to_string(goal));
    }
    template <typename T>
    static LispError unexpectedElement(const std::string& expected, const std::string& in, T& found) {
        return LispError("Expected " + expected + " in " + in + ", found " + found->toString());
    }
};
class EOFError : public std::runtime_error {
public:
    using runtime_error::runtime_error;
};
#endif
