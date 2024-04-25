#include "Parser.h"
#include "Token/Tokenizer.h"
#include "Evaluator.h"
#include <iostream>

// TODO
// 1. Implement the Parser class

int main() {
    std::unordered_map<std::string, std::string> variables;
    Parser parser(std::make_shared<std::unordered_map<std::string, std::string>>(variables));
    Evaluator evaluator(std::make_shared<std::unordered_map<std::string, std::string>>(variables));
    std::vector<std::shared_ptr<Node>> parsed;

    while (true) {
        std::string input;
        std::cout << "Welcome to the fancy new Prompt LISP INTERPRETER, type in LISP commands!";
        std::cout << ">";
        std::getline(std::cin, input);

        if (input == "quit") {
            std::cout << "bye";
            break;
        }

        parsed = parser(input);;
        std::cout << evaluator(parsed) << std::endl;

    }


    return 0;
}
