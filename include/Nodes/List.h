//
// Created by aaron on 4/23/24.
//

#ifndef LISPINTERPRETER_LIST_H
#define LISPINTERPRETER_LIST_H

#include "Nodes.h"
#include "Atoms.h"

#include <memory>

class ListNode : public Node {
private:
    std::vector<std::shared_ptr<Node>>::iterator it;
    std::vector<std::shared_ptr<Node>> nodes;

    std::string evaluate_sum();
    std::string evaluate_sub();
    std::string evaluate_mult();
    std::string evaluate_div();
    std::string evaluate_sqrt();
    std::string evaluate_pow();
    std::string evaluate_car();
    std::string evaluate_cdr();

public:
    ListNode() {
        type = LIST;
    }

    [[maybe_unused]] explicit ListNode(std::vector<std::shared_ptr<Node>> n) : nodes(std::move(n)) {}

    void add_node(std::shared_ptr<Node> n) { nodes.push_back(std::move(n)); }
    [[nodiscard]] const std::vector<std::shared_ptr<Node>>& get_nodes() { return nodes; }

    std::string evaluate() override;

    std::string evaluate_symbol();
};


#endif //LISPINTERPRETER_LIST_H
