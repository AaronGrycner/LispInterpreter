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
#include "Function.h"

class DefineNode : public Node {
private:
    std::string name;
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

class FunctionDefineNode : public Node {
private:
    std::string name;
    std::shared_ptr<FunctionNode> func;
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<FunctionNode>>> functions;

public:
    FunctionDefineNode(std::string n, std::shared_ptr<FunctionNode> a, std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<FunctionNode>>> funcs)
    : name(std::move(n)), func(std::move(a)), functions(std::move(funcs))
    {
        type = DEFUN;
    }

    std::string evaluate() override {
        functions->insert({name, func});
        return "";
    }
};



#endif //LISPINTERPRETER_DEFINE_H
