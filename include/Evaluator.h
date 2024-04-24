//
// Created by aaron on 4/23/24.
//

#ifndef LISPINTERPRETER_EVALUATOR_H
#define LISPINTERPRETER_EVALUATOR_H

#include <memory>
#include "Nodes/Nodes.h"

class Evaluator {
public:
    std::string operator()(const std::vector<std::shared_ptr<Node>>& node);
};


#endif //LISPINTERPRETER_EVALUATOR_H
