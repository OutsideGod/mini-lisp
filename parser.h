#ifndef PARSER_H
#define PARSER_H

#include <deque>
#include "value.h"
#include "tokenizer.h"
#include "error.h"

class Parser {
private:
    std::deque<TokenPtr> tokendeque;

public:
    Parser(std::deque<TokenPtr> tokens);
    ValuePtr parse();
    ValuePtr parseTails();
};
#endif
