//
// Created by mrzee on 4/25/2024.
//

#ifndef LISPINTERPRETER_QUOTENODE_H
#define LISPINTERPRETER_QUOTENODE_H

#include "Nodes.h"
#include "Token/Token.h"

class QuoteNode : public Node {
public:
    explicit QuoteNode(const std::vector<Tokens::Token> &tokens) {
        type = QUOTE;
        for (const auto &token : tokens) {
            value += token.get_value() + " ";
        }
    }

    std::string evaluate() override {
        return value;
    }

};

#endif //LISPINTERPRETER_QUOTENODE_H
