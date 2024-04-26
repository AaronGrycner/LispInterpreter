#ifndef LISPINTERPRETER_TOKEN_H
#define LISPINTERPRETER_TOKEN_H

#include <string>
#include <utility>

namespace Tokens {
    enum TokenType {
        OPEN_PAREN,  // '('
        CLOSE_PAREN, // ')'
        NUMBER,      // Numerical literals e.g., 123, 3.14
        SYMBOL,      // Identifiers and function names e.g., x, myFunc
        BOOLEAN,     // Boolean values, e.g., T, NIL
        QUOTE,       // The quote symbol, e.g., '
        DEFINE,      // Define symbol, e.g., define
        SET,         // Set! symbol for variable assignment
        CONDITIONAL,          // If conditional
        LAMBDA,      // Lambda function definition
        DEFUN        // Function definition
    };

    class Token {
        TokenType type;
        std::string value;

    public:
        Token(TokenType type, std::string value) : type(type), value(std::move(value)) {}

        [[nodiscard]] TokenType get_type() const { return type; }
        [[nodiscard]] std::string get_value() const { return value; }
    };
}

#endif //LISPINTERPRETER_TOKEN_H
