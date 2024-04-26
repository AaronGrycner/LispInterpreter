//
// Created by aaron on 4/23/24.
//

#ifndef LISPINTERPRETER_NODES_H
#define LISPINTERPRETER_NODES_H

#include <vector>
#include <string>

enum NodeType {
    SYMBOL,
    LIST,
    BOOLEAN,
    NUMBER,
    CONDITIONAL,
    DEFINE,
    QUOTE,
    DEFUN,
    FUNCTION,
    SET
};

class Node {
protected:
    std::string value;
    NodeType type;

public:
    virtual std::string evaluate() = 0;
    virtual ~Node() = default;

    [[nodiscard]] virtual std::string get_value() const { return value; }
    [[nodiscard]] NodeType get_type() const { return type; }
};


#endif //LISPINTERPRETER_NODES_H
