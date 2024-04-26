#ifndef LISPINTERPRETER_FUNCTION_H
#define LISPINTERPRETER_FUNCTION_H

#include <vector>
#include <memory>
#include <stdexcept>
#include "Nodes.h"
#include "Atoms.h"
#include "List.h"
#include <iostream>

class FunctionNode : public Node {
private:
    int num_args;
    std::vector<std::shared_ptr<Node>> arguments;  // Arguments should be a vector of Nodes
    std::shared_ptr<SymbolNode> operation;

public:
    FunctionNode(int args, std::shared_ptr<SymbolNode> op) : operation(std::move(op)), num_args(args) {
        type = FUNCTION;
    }

    std::string evaluate() override {
        auto it = arguments.begin();
        for (const auto& arg : arguments) {
            if (arg->get_type() == LIST) {
                std::string buffer;
                buffer = arg->evaluate();
                auto new_node = std::make_shared<NumberNode>(stoi(buffer));
                arguments.erase(it);
                arguments.insert(it, new_node);
            }
            ++it;
        }

        arguments.insert(arguments.begin(), operation);
        auto eval_node = ListNode(arguments);
        arguments.clear();
        return eval_node.evaluate();
    }

    [[nodiscard]] size_t get_num_args() const {
        return num_args;
    }

    void set_argument(const std::shared_ptr<Node>& arg) {
        arguments.push_back(arg);
    }
};

#endif // LISPINTERPRETER_FUNCTION_H
