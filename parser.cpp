#include "parser.h"

Parser::Parser(std::deque<TokenPtr> tokens) {
    tokendeque = std::move(tokens);
}

ValuePtr Parser::parseTails() {
    if (tokendeque.front()->getType() == TokenType::RIGHT_PAREN) {
        tokendeque.pop_front();
        return std::make_shared<NilValue>();
    }
    auto car = this->parse();
    if (tokendeque.front()->getType() == TokenType::DOT) {
        tokendeque.pop_front();
        auto cdr = this->parse();
        if (tokendeque.front()->getType() == TokenType::RIGHT_PAREN) {
            tokendeque.pop_front();
        } 
        else {
            throw SyntaxError("SyntaxError: Expected exactly one element after .");
        }
        return std::make_shared<PairValue>(car, cdr);
    } else {
        auto cdr = this->parseTails();
        return std::make_shared<PairValue>(car, cdr);
    }
}

ValuePtr Parser::parse() {
    auto token = std::move(tokendeque.front());
    tokendeque.pop_front();
    if (token->getType() == TokenType::NUMERIC_LITERAL) {
        auto value = static_cast<NumericLiteralToken&>(*token).getValue();
        return std::make_shared<NumericValue>(value);
    }
    else if (token->getType() == TokenType::BOOLEAN_LITERAL) {
        auto value = static_cast<BooleanLiteralToken&>(*token).getValue();
        return std::make_shared<BooleanValue>(value);
    }
    else if (token->getType() == TokenType::STRING_LITERAL) {
        auto value = static_cast<StringLiteralToken&>(*token).getValue();
        return std::make_shared<StringValue>(value);
    }
    else if (token->getType() == TokenType::IDENTIFIER) {
        auto value = static_cast<IdentifierToken&>(*token).getName();
        return std::make_shared<SymbolValue>(value);
    } 
    else if (token->getType() == TokenType::LEFT_PAREN) {
        return this->parseTails();
    }
    else if (token->getType() == TokenType::QUOTE) {
        return std::make_shared<PairValue>(
            std::make_shared<SymbolValue>("quote"),
            std::make_shared<PairValue>(this->parse(),
                                        std::make_shared<NilValue>()));
    } 
    else if (token->getType() == TokenType::QUASIQUOTE) {
        return std::make_shared<PairValue>(
            std::make_shared<SymbolValue>("quasiquote"),
            std::make_shared<PairValue>(this->parse(),
                                        std::make_shared<NilValue>()));
    } 
    else if (token->getType() == TokenType::UNQUOTE) {
        return std::make_shared<PairValue>(
            std::make_shared<SymbolValue>("unquote"),
            std::make_shared<PairValue>(this->parse(),
                                        std::make_shared<NilValue>()));
    }
    else if (token->getType() == TokenType::RIGHT_PAREN) {
        throw SyntaxError("SyntaxError: Unexpected token (RIGHT_PAREN)");
    }
    else if (token->getType() == TokenType::DOT) {
        throw SyntaxError("SyntaxError: Unexpected token (DOT)");
    }
    else {
        throw SyntaxError("SyntaxError: Unexpected token");
    }
}

