#include "Parser.h"
#include "include/Tokenizer.h"
#include "Evaluator.h"
#include <iostream>
#include <fstream>
#include <sstream>

// TODO
// 1. Implement the Parser class

int main() {
    std::ofstream file("output.txt");

    std::unordered_map<std::string, std::string> variables;
    std::unordered_map<std::string, std::shared_ptr<FunctionNode>> functions;
    Parser parser(std::make_shared<std::unordered_map<std::string, std::string>>(variables),
                  std::make_shared<std::unordered_map<std::string, std::shared_ptr<FunctionNode>>>(functions));
    Evaluator evaluator(std::make_shared<std::unordered_map<std::string, std::string>>(variables),
                        std::make_shared<std::unordered_map<std::string, std::shared_ptr<FunctionNode>>>(functions));
    std::vector<std::shared_ptr<Node>> parsed;

    std::cout << "Welcome to the fancy new Prompt LISP INTERPRETER, type in LISP commands!";

    while (true) {
        std::stringstream st;
        std::string input;
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

        try {
            parsed = parser(input);
            st << evaluator(parsed) << std::endl;
            std::cout << st.str();
        }
        catch (std::runtime_error &e) {
            std::cout << e.what() << std::endl;
            file << e.what() << std::endl;
        }

        file << st.str();
    }

    return 0;
}
