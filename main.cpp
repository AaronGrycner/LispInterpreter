#include "Parser.h"
#include "Token/Tokenizer.h"
#include "Evaluator.h"
#include <iostream>

// TODO
// 1. Implement the Parser class

int main() {
    std::unordered_map<std::string, std::string> variables;
    std::unordered_map<std::string, std::shared_ptr<FunctionNode>> functions;
    Parser parser(std::make_shared<std::unordered_map<std::string, std::string>>(variables),
                  std::make_shared<std::unordered_map<std::string, std::shared_ptr<FunctionNode>>>(functions));
    Evaluator evaluator(std::make_shared<std::unordered_map<std::string, std::string>>(variables),
                        std::make_shared<std::unordered_map<std::string, std::shared_ptr<FunctionNode>>>(functions));
    std::vector<std::shared_ptr<Node>> parsed;

    while (true) {
        std::string input;
        std::cout << "Welcome to the fancy new Prompt LISP INTERPRETER, type in LISP commands!";
        std::cout << ">";
        std::getline(std::cin, input);

        // Check for non-ASCII characters
        try {
            for (unsigned char c: input) {
                if (c > 127) {  // Non-ASCII characters have values greater than 127
                    throw std::runtime_error("Input contains non-ASCII characters");
                }
            }
        } catch (std::runtime_error &e) {
            std::cout << e.what() << std::endl;
            continue;
        }

        if (input == "quit") {
            std::cout << "bye";
            break;
        }

        parsed = parser(input);
        std::cout << evaluator(parsed) << std::endl;

    }

    return 0;
}
