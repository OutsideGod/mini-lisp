#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "./tokenizer.h"
#include "value.h"
#include "parser.h"
#include "rjsj_test.hpp"
#include "eval_env.h"

struct TestCtx {
    std::shared_ptr<EvalEnv> env = EvalEnv::createGlobal();
    std::string eval(std::string input) {
        auto tokens = Tokenizer::tokenize(input);
        Parser parser(std::move(tokens));
        auto value = parser.parse();
        auto result = env->eval(std::move(value));
        return result->toString();
    }
};
void runRepl() {
    std::shared_ptr<EvalEnv> env = EvalEnv::createGlobal();
    while (true) {
        try {
            std::cout << ">>> ";
            std::string line;
            if (!std::getline(std::cin, line)) break;
            if (line.empty()) continue;
            auto tokens = Tokenizer::tokenize(line);
            Parser parser(std::move(tokens));
            auto value = parser.parse();
            auto result = env->eval(std::move(value));
            std::cout << result->toString() << std::endl;
        }
        catch (std::runtime_error& e) {
            std::cerr <<e.what() << std::endl;
        }
    }
}
void runFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        std::exit(1);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    file.close();

    auto env = EvalEnv::createGlobal();
    auto tokens = Tokenizer::tokenize(content);
    Parser parser(std::move(tokens));
    while (true) {
        try {
            ValuePtr expr = parser.parse();
            env->eval(std::move(expr));
        }
        catch (EOFError&) {
            break;
        }
        catch (std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
            std::exit(1);
        }
    }
}
int main(int argc, char* argv[]) {
    RJSJ_TEST(TestCtx, Lv2, Lv3, Lv4, Lv5, Lv5Extra, Lv6, Lv7, Lv7Lib, Sicp);
    if (argc > 1) {
        runFile(argv[1]);
    }
    else {
        runRepl();
    }
    return 0;
}
