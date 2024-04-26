#include <sstream>
#include <cctype>
#include <cstdint>
#include "include/Token/Tokenizer.h"

using namespace Tokens;

std::vector<Token> Tokenizer::tokenize(const std::string &input) {
    std::vector<Token> tokens;
    std::istringstream iss(input);
    char ch;
    std::string buffer;

    while (iss >> ch) {
        if (std::isspace(ch)) continue; // Ignore whitespace

        if (std::isdigit(ch) || (ch == '-' && std::isdigit(iss.peek()))) {
            // Handle numbers
            iss.putback(ch);
            int32_t num;
            iss >> num;
            tokens.emplace_back(TokenType::NUMBER, std::to_string(num));
        } else {
            switch (ch) {
                case '(':
                    tokens.emplace_back(TokenType::OPEN_PAREN, "(");
                    break;
                case ')':
                    tokens.emplace_back(TokenType::CLOSE_PAREN, ")");
                    break;
                case '\'':
                    tokens.emplace_back(TokenType::QUOTE, "'");
                    break;
                case '>':
                    tokens.emplace_back(TokenType::RELATION, ">");
                    break;
                case '<':
                    tokens.emplace_back(TokenType::RELATION, "<");
                    break;
                case '=':
                    tokens.emplace_back(TokenType::RELATION, "=");
                    break;
                default:
                    // Handle other symbols as identifiers or error
                    buffer.clear();
                    buffer += ch;

                    while (iss.peek() != EOF && !std::isspace(iss.peek()) && iss.peek() != '(' && iss.peek() != ')') {
                        iss.get(ch);  // Use get to fetch the next character without skipping it based on conditions
                        buffer += ch;
                    }

                    // Keyword or symbol differentiation
                    if (buffer == "define") tokens.emplace_back(TokenType::DEFINE, buffer);

                    else if (buffer == "car") tokens.emplace_back(TokenType::CAR, buffer);
                    else if (buffer == "cdr") tokens.emplace_back(TokenType::CDR, buffer);

                    else if (buffer == "and") tokens.emplace_back(TokenType::RELATION, buffer);
                    else if (buffer == "or") tokens.emplace_back(TokenType::RELATION, buffer);
                    else if (buffer == "not") tokens.emplace_back(TokenType::RELATION, buffer);
                    else if (buffer == "!=") tokens.emplace_back(TokenType::RELATION, buffer);

                    else if (buffer == "+") tokens.emplace_back(TokenType::SYMBOL, buffer);
                    else if (buffer == "-") tokens.emplace_back(TokenType::SYMBOL, buffer);
                    else if (buffer == "*") tokens.emplace_back(TokenType::SYMBOL, buffer);
                    else if (buffer == "/") tokens.emplace_back(TokenType::SYMBOL, buffer);

                    else if (buffer == "quote") tokens.emplace_back(TokenType::QUOTE, buffer);

                    else if (buffer == "set!") tokens.emplace_back(TokenType::SET, buffer);

                    else if (buffer == "if") tokens.emplace_back(TokenType::CONDITIONAL, buffer);

                    else if (buffer == "lambda") tokens.emplace_back(TokenType::LAMBDA, buffer);

                    else if (buffer == "defun") tokens.emplace_back(TokenType::DEFUN, buffer);

                    else if (buffer == "T" || buffer == "NIL") tokens.emplace_back(TokenType::BOOLEAN, buffer);

                    else tokens.emplace_back(TokenType::SYMBOL, buffer);

                    break;
            }
        }
    }
    return tokens;
}