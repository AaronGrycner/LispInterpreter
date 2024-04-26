//
// Created by aaron on 4/25/24.
//

#ifndef LISPINTERPRETER_RELATION_H
#define LISPINTERPRETER_RELATION_H

#include <memory>
#include <stdexcept>
#include "Nodes.h"

class RelationNode : public Node {
private:
    std::string oper;
    std::shared_ptr<Node> left, right;

public:
    RelationNode(std::string o, std::shared_ptr<Node> l, std::shared_ptr<Node> r) : oper(std::move(o)), left(std::move(l)), right(std::move(r)) {
        type = RELATION;
    }

    std::string evaluate() override {
        auto l = left->evaluate();
        auto r = right->evaluate();

        if (oper == "=") {
            return l == r ? "T" : "NIL";
        } else if(oper == "<") {
            return l < r ? "T" : "NIL";
        } else if(oper == ">") {
            return l > r ? "T" : "NIL";
        } else if(oper == "<=") {
            return l <= r ? "T" : "NIL";
        } else if(oper == ">=") {
            return l >= r ? "T" : "NIL";
        } else if(oper == "!=") {
            return l != r ? "T" : "NIL";
        } else if(oper == "and") {
            return (l == "T" && r == "T") ? "T" : "NIL";
        } else if(oper == "or") {
            return (l == "T" || r == "T") ? "T" : "NIL";
        } else if(oper == "not") {
            return l == "NIL" ? "T" : "NIL";
        } else {
            throw std::runtime_error("Invalid operator " + oper);
        }

    }
};

#endif //LISPINTERPRETER_RELATION_H
