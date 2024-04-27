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
        for (const auto &arg: arguments) {
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

    void set_argument(const std::shared_ptr<Node> &arg) {
        arguments.push_back(arg);
    }

    std::string get_operation() {
        return operation->get_value();
    }
};

class MapcarNode : public Node {
private:
    std::shared_ptr<SymbolNode> operation;
    std::shared_ptr<ListNode> list;
public:
    MapcarNode(std::shared_ptr<SymbolNode> o, std::shared_ptr<ListNode> lst) : operation(std::move(o)),
                                                                                list(std::move(lst)) {
        type = MAPCAR;
    }

    std::string evaluate() override {
        std::string buffer;
        std::vector<std::shared_ptr<ListNode>> args;

        for (const auto &lst: list->get_nodes()) {
            auto new_node = std::make_shared<ListNode>();
            new_node->add_node(operation);
            new_node->add_node(lst);
            args.push_back(new_node);
        }

        for (auto &arg: args) {
            buffer += arg->evaluate() + " ";
        }

        return buffer;
    }
};


#endif // LISPINTERPRETER_FUNCTION_H
