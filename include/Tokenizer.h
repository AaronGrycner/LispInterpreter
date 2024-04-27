//
// Created by aaron on 4/23/24.
//

#ifndef LISPINTERPRETER_TOKENIZER_H
#define LISPINTERPRETER_TOKENIZER_H

#include <vector>
#include "Token.h"

class Tokenizer {
public:
    static std::vector<Tokens::Token> tokenize(const std::string &input);
};


#endif //LISPINTERPRETER_TOKENIZER_H
