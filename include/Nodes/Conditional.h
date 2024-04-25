//
// Created by aaron on 4/24/24.
//

#include <memory>
#include <stdexcept>
#include "Nodes.h"
#include "Atoms.h"

#ifndef LISPINTERPRETER_CONDITIONAL_H
#define LISPINTERPRETER_CONDITIONAL_H

class ConditionalNode : public Node {
private:
    std::shared_ptr<Node> condition;
    std::shared_ptr<Node> true_branch;
    std::shared_ptr<Node> false_branch;

public:
    ConditionalNode(std::shared_ptr<Node> c, std::shared_ptr<Node> t, std::shared_ptr<Node> f)
            : condition(std::move(c)), true_branch(std::move(t)), false_branch(std::move(f)) {
        type = CONDITIONAL;
    }

    std::string evaluate() override {
        // Evaluate the condition and choose the branch based on its boolean result
        if (condition->evaluate() == "T") {
            return true_branch->evaluate();
        } else {
            return false_branch->evaluate();
        }
    }
};


class ConditionNode : public Node {
private:
    std::string operator_;
    std::shared_ptr<Node> leftOperand;
    std::shared_ptr<Node> rightOperand;

public:
    // Constructor assumes operator is a simple string
    ConditionNode(const std::string& op, std::shared_ptr<Node> left, std::shared_ptr<Node> right)
            : operator_(op), leftOperand(std::move(left)), rightOperand(std::move(right)) {
        type = CONDITIONAL;
    }

    std::string evaluate() override {
        int left = std::stoi(leftOperand->evaluate());
        int right = std::stoi(rightOperand->evaluate());

        if (operator_ == ">") {
            return left > right ? "T" : "NIL";
        } else if (operator_ == "<") {
            return left < right ? "T" : "NIL";
        } else if (operator_ == "=") {
            return left == right ? "T" : "NIL";
        } else if (operator_ == "!=") {
            return left != right ? "T" : "NIL";
        } else if (operator_ == "and") {
            return (left != 0 && right != 0) ? "T" : "NIL";
        } else if (operator_ == "or") {
            return (left != 0 || right != 0) ? "T" : "NIL";
        } else if (operator_ == "not") {
            // Handle 'not' correctly if only one operand should be present
            return (left == 0) ? "T" : "NIL";
        } else {
            throw std::runtime_error("Unsupported operator: " + operator_);
        }
    }
};


#endif //LISPINTERPRETER_CONDITIONAL_H
