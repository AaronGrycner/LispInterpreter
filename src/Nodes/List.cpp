#include "../../include/Nodes/List.h"
#include <stdexcept>
#include <iomanip>
#include <sstream>

std::string ListNode::evaluate() {
    if (nodes.empty()) {
        return "Empty list";
    }

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
        return "Unsupported operation: " + symbol;
    }
}

std::string ListNode::evaluate_sum() {
    float ans = 0;
    try {
        for (size_t i = 1; i < nodes.size(); i++) {
            ans += std::stof(nodes[i]->evaluate());
        }
    } catch (const std::invalid_argument& e) {
        return "Error: Non-numeric argument in addition";
    }
    return format_float(ans);
}

std::string ListNode::evaluate_sub() {
    if (nodes.size() < 2) return "Error: Insufficient arguments for subtraction";
    float ans;
    try {
        ans = std::stof(nodes[1]->evaluate());
        for (size_t i = 2; i < nodes.size(); i++) {
            ans -= std::stof(nodes[i]->evaluate());
        }
    } catch (const std::invalid_argument& e) {
        return "Error: Non-numeric argument in subtraction";
    }
    return format_float(ans);
}

std::string ListNode::evaluate_mult() {
    float ans = 1;
    try {
        for (size_t i = 1; i < nodes.size(); i++) {
            ans *= std::stof(nodes[i]->evaluate());
        }
    } catch (const std::invalid_argument& e) {
        return "Error: Non-numeric argument in multiplication";
    }
    return format_float(ans);
}

std::string ListNode::evaluate_div() {
    if (nodes.size() < 2) return "Error: Insufficient arguments for division";
    float ans;
    try {
        ans = std::stof(nodes[1]->evaluate());
        for (size_t i = 2; i < nodes.size(); i++) {
            float divisor = std::stof(nodes[i]->evaluate());
            if (divisor == 0) return "Error: Division by zero";
            ans /= divisor;
        }
    } catch (const std::invalid_argument& e) {
        return "Error: Non-numeric argument in division";
    }
    return format_float(ans);
}

std::string ListNode::format_float(float value) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << value;
    return oss.str();
}