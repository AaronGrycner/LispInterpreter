#include <sstream>
#include "Evaluator.h"
#include "Nodes/List.h"

std::string Evaluator::operator()(const std::vector<std::shared_ptr<Node>>& nodes) {
    std::stringstream result;

    for (const auto& current_node : nodes) {
        result << current_node->evaluate() << " "; // Append a space as a separator
    }
    return result.str();
}