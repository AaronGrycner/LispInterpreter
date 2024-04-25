//
// Created by aaron on 4/23/24.
//

#ifndef LISPINTERPRETER_EVALUATOR_H
#define LISPINTERPRETER_EVALUATOR_H

#include <memory>
#include <unordered_map>

#include "Nodes/Nodes.h"
#include "Nodes/Define.h"

class Evaluator {
private:
    std::unordered_map<std::string, std::string> variables;

public:
    std::string operator()(const std::vector<std::shared_ptr<Node>>& node);

};


#endif //LISPINTERPRETER_EVALUATOR_H
