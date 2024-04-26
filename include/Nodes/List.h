//
// Created by aaron on 4/23/24.
//

#ifndef LISPINTERPRETER_LIST_H
#define LISPINTERPRETER_LIST_H

#include "Nodes.h"
#include "Atoms.h"

#include <memory>

class ListNode : public Node {
protected:
    std::vector<std::shared_ptr<Node>>::iterator it;
    std::vector<std::shared_ptr<Node>> nodes;

    std::string evaluate_sum();
    std::string evaluate_sub();
    std::string evaluate_mult();
    std::string evaluate_div();
    std::string evaluate_sqrt();
    std::string evaluate_pow();
    std::string evaluate_as_string();

public:
    ListNode() {
        type = LIST;
    }

    [[maybe_unused]] explicit ListNode(std::vector<std::shared_ptr<Node>> n) : nodes(std::move(n)) {}

    void add_node(std::shared_ptr<Node> n) { nodes.push_back(std::move(n)); }
    [[nodiscard]] const std::vector<std::shared_ptr<Node>>& get_nodes() { return nodes; }

    std::string evaluate() override;

    std::string evaluate_symbol();

    size_t get_nodes_size() { return nodes.size(); }
};

class CarNode : public ListNode {
public:
    CarNode() {
        type = CAR;
    }

    std::string evaluate() override {
        return nodes.at(0)->evaluate();
    }
};

class CdrNode : public ListNode {
public:
    CdrNode() {
        type = CDR;
    }

    std::string evaluate() override {
        std::string buffer;

        for (auto it = nodes.begin() + 1; it != nodes.end(); ++it) {
            buffer += (*it)->evaluate() + " ";
        }

        return buffer;
    }
};

#endif //LISPINTERPRETER_LIST_H
