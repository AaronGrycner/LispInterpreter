//
// Created by aaron on 4/24/24.
//

#ifndef LISPINTERPRETER_DEFINE_H
#define LISPINTERPRETER_DEFINE_H

#include <memory>
#include <utility>
#include <unordered_map>
#include <stdexcept>

#include "Nodes.h"
#include "Atoms.h"

class DefineNode : public Node {
private:
    std:: string name;
    std::shared_ptr<Node> value;
    std::shared_ptr<std::unordered_map<std::string, std::string>> variables;

public:
    DefineNode(std::string n, std::shared_ptr<Node> v, std::shared_ptr<std::unordered_map<std::string, std::string>> vars) : name(std::move(n)), value(std::move(v)), variables(std::move(vars)) {
        type = DEFINE;
    }

    std::string evaluate() override {
        variables->insert({name, value->evaluate()});
        return "";
    }
};

#endif //LISPINTERPRETER_DEFINE_H
