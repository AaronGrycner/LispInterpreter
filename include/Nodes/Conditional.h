//
// Created by aaron on 4/24/24.
//

#include <memory>
#include "Nodes.h"
#include "Atoms.h"

#ifndef LISPINTERPRETER_CONDITIONAL_H
#define LISPINTERPRETER_CONDITIONAL_H

#endif //LISPINTERPRETER_CONDITIONAL_H


class ConditionalNode : public Node {
private:
    std::shared_ptr<Node> condition;
    std::shared_ptr<Node> true_branch;
    std::shared_ptr<Node> false_branch;

public:
    ConditionalNode(std::shared_ptr<Node> c, std::shared_ptr<Node> t, std::shared_ptr<Node> f) : condition(std::move(c)), true_branch(std::move(t)), false_branch(std::move(f)) {
        type = CONDITIONAL;
    }

    std::string evaluate() override {
        if (condition->evaluate() == "T") {
            return true_branch->evaluate();
        } else {
            return false_branch->evaluate();
        }
    }
};

// represents a condition eg (> 4 3), that is owned by a conditional node, which determines what happens when the condition is true or false
class ConditionNode : public Node {
private:
    std::shared_ptr<SymbolNode> oper;
    std::shared_ptr<NumberNode> left, right;

public:
    ConditionNode(std::shared_ptr<SymbolNode> o, std::shared_ptr<NumberNode> l, std::shared_ptr<NumberNode> r) : oper(std::move(o)), left(std::move(l)), right(std::move(r)) {
        type = NodeType::CONDITIONAL;
    }

    std::string evaluate() override {
        if (oper->get_value() == ">") {
            return left->evaluate() > right->evaluate() ? "T" : "NIL";
        } else if (oper->get_value() == "<") {
            return left->evaluate() < right->evaluate() ? "T" : "NIL";
        } else if (oper->get_value() == "=") {
            return left->evaluate() == right->evaluate() ? "T" : "NIL";
        } else if (oper->get_value() == ">=") {
            return left->evaluate() >= right->evaluate() ? "T" : "NIL";
        } else if (oper->get_value() == "<=") {
            return left->evaluate() <= right->evaluate() ? "T" : "NIL";
        } else {
            return "NIL";
        }
    }
};