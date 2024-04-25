#include <sstream>
#include <cctype>
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
            double num;
            iss >> num;
            tokens.emplace_back(TokenType::NUMBER, std::to_string(num));
        }

        else {
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
                    tokens.emplace_back(TokenType::SYMBOL, ">");
                    break;
                case '<':
                    tokens.emplace_back(TokenType::SYMBOL, "<");
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