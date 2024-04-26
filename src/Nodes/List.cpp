#include "../../include/Nodes/List.h"
#include <stdexcept>
#include <iomanip>
#include <sstream>
#include <iostream>

std::string ListNode::evaluate() {
    if (nodes.empty()) {
        return "Empty list";
    }

    switch (nodes.at(0)->get_type()) {
        case NodeType::SYMBOL:
            return evaluate_symbol();
        case NodeType::CONDITIONAL:
            return nodes.at(0)->evaluate();
        case NodeType::DEFINE:
            return nodes.at(0)->evaluate();
        case NodeType::DEFUN:
            return nodes.at(0)->evaluate();
        case NodeType::FUNCTION:
            return nodes.at(0)->evaluate();
        default:
            throw std::runtime_error("Unexpected token type in list");
    }
}

std::string ListNode::evaluate_symbol() {
    std::string symbol = nodes.at(0)->get_value();
    if (symbol == "+") {
        return evaluate_sum();
    } else if (symbol == "-") {
        return evaluate_sub();
    } else if (symbol == "*") {
        return evaluate_mult();
    } else if (symbol == "/") {
        return evaluate_div();
    } else {
        throw std::runtime_error("Unsupported operation: " + symbol);
    }
}

std::string ListNode::evaluate_sum() {
    int32_t ans = 0;
    try {
        for (size_t i = 1; i < nodes.size(); i++) {
            ans += std::stoi(nodes[i]->evaluate());
        }
    } catch (const std::invalid_argument &e) {
        return "Error: Non-numeric argument in addition";
    }
    return std::to_string(ans);
}

std::string ListNode::evaluate_sub() {
    if (nodes.size() < 2) return "Error: Insufficient arguments for subtraction";
    int32_t ans;
    try {
        ans = std::stoi(nodes[1]->evaluate());
        for (size_t i = 2; i < nodes.size(); i++) {
            ans -= std::stoi(nodes[i]->evaluate());
        }
    } catch (const std::invalid_argument &e) {
        return "Error: Non-numeric argument in subtraction";
    }
    return std::to_string(ans);
}

std::string ListNode::evaluate_mult() {
    int32_t ans = 1;
    try {
        for (size_t i = 1; i < nodes.size(); i++) {
            ans *= std::stoi(nodes[i]->evaluate());
        }
    } catch (const std::invalid_argument &e) {
        return "Error: Non-numeric argument in multiplication";
    }
    return std::to_string(ans);
}

std::string ListNode::evaluate_div() {
    if (nodes.size() < 2) return "Error: Insufficient arguments for division";
    int32_t ans;
    try {
        ans = std::stoi(nodes[1]->evaluate());
        for (size_t i = 2; i < nodes.size(); i++) {
            int32_t divisor = std::stoi(nodes[i]->evaluate());
            if (divisor == 0) return "Error: Division by zero";
            ans /= divisor;
        }
    } catch (const std::invalid_argument &e) {
        return "Error: Non-numeric argument in division";
    }
    return std::to_string(ans);
}
