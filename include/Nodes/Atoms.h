//
// Created by aaron on 4/23/24.
//

#ifndef LISPINTERPRETER_ATOMS_H
#define LISPINTERPRETER_ATOMS_H

#include "Nodes.h"

class AtomNode : public Node {
    public:
    std::string evaluate() override {
        return value;
    }
};

//
// NUMBER
//

class NumberNode : public AtomNode {
private:
    float number;

public:
    explicit NumberNode(float v) : number(v) {
        type = NodeType::NUMBER;
        value = std::to_string(v);
    }
};


//
// SYMBOL
//

class SymbolNode : public AtomNode {
private:
    std::string symbol;

public:
    explicit SymbolNode(std::string value) : symbol(std::move(value)) {
        type = NodeType::SYMBOL;
        this->value = symbol;
    }
};


//
// BOOLEAN
//

class BooleanNode : public AtomNode {
private:
    bool boolean;

public:
    explicit BooleanNode(bool v) : boolean(v) {
        type = NodeType::BOOLEAN;
        value = v ? "T" : "NIL";
    }
};

#endif //LISPINTERPRETER_ATOMS_H
