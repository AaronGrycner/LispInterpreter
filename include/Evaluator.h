//
// Created by aaron on 4/23/24.
//

#ifndef LISPINTERPRETER_EVALUATOR_H
#define LISPINTERPRETER_EVALUATOR_H

#include <memory>
#include <unordered_map>

#include "Nodes.h"

class Evaluator {
private:
    std::shared_ptr<std::unordered_map<std::string, std::string>> variables;
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<FunctionNode>>> functions;

public:
    explicit Evaluator(std::shared_ptr<std::unordered_map<std::string, std::string>> vars,
                       std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<FunctionNode>>> funcs)
    : variables(std::move(vars)),
    functions(std::move(funcs)) {}

    std::string operator()(const std::vector<std::shared_ptr<Node>>& node);

};


#endif //LISPINTERPRETER_EVALUATOR_H
