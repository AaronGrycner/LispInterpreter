#include "../../include/Nodes/List.h"
#include <stdexcept>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <complex>

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
        case NodeType::SET:
            return nodes.at(0)->evaluate();
        case NodeType::RELATION:
            return nodes.at(0)->evaluate();
        case NodeType::CAR:
            return nodes.at(0)->evaluate();
        case NodeType::CDR:
            return nodes.at(0)->evaluate();
        case NodeType::CONS:
            return nodes.at(0)->evaluate();
        default:
            throw std::runtime_error("Unexpected node type in list");
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
    } else if (symbol == "sqrt") {
        return evaluate_sqrt();
    } else if (symbol == "pow") {
        return evaluate_pow();
    } else {
        return evaluate_as_string();
    }
}

std::string ListNode::evaluate_sum() {
    int32_t ans = 0;
    try {
        for (size_t i = 1; i < nodes.size(); i++) {
            ans += std::stoi(nodes[i]->evaluate());
        }
    } catch (const std::invalid_argument &e) {
        throw std::runtime_error("Error: Non-numeric argument in addition");
    }
    return std::to_string(ans);
}

std::string ListNode::evaluate_sub() {
    if (nodes.size() < 2) throw std::runtime_error("Error: Insufficient arguments for subtraction");
    int32_t ans;
    try {
        ans = std::stoi(nodes[1]->evaluate());
        for (size_t i = 2; i < nodes.size(); i++) {
            ans -= std::stoi(nodes[i]->evaluate());
        }
    } catch (const std::invalid_argument &e) {
        throw std::runtime_error("Error: Non-numeric argument in subtraction");
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
        throw std::runtime_error("Error: Non-numeric argument in multiplication");
    }
    return std::to_string(ans);
}

std::string ListNode::evaluate_div() {
    if (nodes.size() < 2) throw std::runtime_error("Invalid number of arguments for division");
    int32_t ans;
    try {
        ans = std::stoi(nodes[1]->evaluate());
        for (size_t i = 2; i < nodes.size(); i++) {
            int32_t divisor = std::stoi(nodes[i]->evaluate());
            if (divisor == 0) throw std::runtime_error("Divide by zero");
            ans /= divisor;
        }
    } catch (const std::invalid_argument &e) {
        throw std::runtime_error("Non-numeric argument in division");
    }
    return std::to_string(ans);
}

std::string ListNode::evaluate_sqrt() {
    if (nodes.size() != 2) {
        throw std::runtime_error("Invalid number of arguments for sqrt function");
    }

    int32_t operand;

    try {
        operand = std::stoi(nodes[1]->evaluate());
    } catch (const std::invalid_argument &e) {
        throw std::runtime_error("Non-numeric argument in sqrt");
    } catch (const std::out_of_range &e) {
        throw std::runtime_error("Argument out of range in sqrt");
    }

    if (operand < 0) {
        throw std::runtime_error("Cannot compute sqrt of a negative number");
    }

    int32_t result = static_cast<int32_t>(std::sqrt(operand));
    return std::to_string(result);
}


std::string ListNode::evaluate_pow() {
    if (nodes.size() != 3) {
        throw std::runtime_error("Invalid number of arguments for pow function; requires exactly two arguments");
    }

    int32_t base, exponent;
    try {
        base = std::stoi(nodes[1]->evaluate());
        exponent = std::stoi(nodes[2]->evaluate());
    } catch (const std::invalid_argument &e) {
        throw std::runtime_error("Non-numeric argument in pow");
    } catch (const std::out_of_range &e) {
        throw std::runtime_error("Argument out of range in pow");
    }

    if (exponent < 0) {
        throw std::runtime_error("Negative exponent in pow");
    }

    int32_t result = 1;
    while (exponent != 0) {
        if (exponent % 2 == 1) {
            if (result > INT32_MAX / base) { // Check for overflow
                throw std::runtime_error("Integer overflow in pow");
            }
            result *= base;
        }
        exponent /= 2;
        if (exponent != 0) {
            if (base > INT32_MAX / base) { // Check for overflow in next step of squaring base
                throw std::runtime_error("Integer overflow in pow");
            }
            base *= base;
        }
    }

    return std::to_string(result);
}

std::string ListNode::evaluate_as_string() {
    std::string buffer;

    for (auto & node : nodes) {
        buffer += node->evaluate() + " ";
    }

    return buffer;
}