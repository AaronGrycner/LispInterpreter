//
// Created by aaron on 4/23/24.
//

#ifndef LISPINTERPRETER_NODES_H
#define LISPINTERPRETER_NODES_H

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include "Token.h"

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
    SET,
    RELATION,
    CAR,
    CDR,
    CONS,
    MAPCAR,
    LAMBDA
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
    int32_t number;

public:
    explicit NumberNode(int32_t v) : number(v) {
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

class ListNode : public Node {
protected:
    std::vector<std::shared_ptr<Node>>::iterator it;
    std::vector<std::shared_ptr<Node>> nodes;

    std::string evaluate_sum();
    std::string evaluate_sub();
    std::string evaluate_mult();
    std::string evaluate_div();
    std::string evaluate_sqrt();
    std::string evaluate_pow();
    std::string evaluate_as_string();

public:
    ListNode() {
        type = LIST;
    }

    [[maybe_unused]] explicit ListNode(std::vector<std::shared_ptr<Node>> n) : nodes(std::move(n)) {}

    void add_node(std::shared_ptr<Node> n) { nodes.push_back(std::move(n)); }
    [[nodiscard]] const std::vector<std::shared_ptr<Node>>& get_nodes() { return nodes; }

    std::string evaluate() override;

    std::string evaluate_symbol();

    size_t get_nodes_size() { return nodes.size(); }
};

class CarNode : public ListNode {
public:
    CarNode() {
        type = CAR;
    }

    std::string evaluate() override {
        return nodes.at(0)->evaluate();
    }
};

class CdrNode : public ListNode {
public:
    CdrNode() {
        type = CDR;
    }

    std::string evaluate() override {
        std::string buffer;

        for (auto it = nodes.begin() + 1; it != nodes.end(); ++it) {
            buffer += (*it)->evaluate() + " ";
        }

        return buffer;
    }
};

class FunctionNode : public Node {
private:
    int num_args;
    std::vector<std::shared_ptr<Node>> arguments;  // Arguments should be a vector of Nodes
    std::shared_ptr<SymbolNode> operation;

public:
    FunctionNode(int args, std::shared_ptr<SymbolNode> op) : operation(std::move(op)), num_args(args) {
        type = FUNCTION;
    }

    std::string evaluate() override {
        auto it = arguments.begin();
        for (const auto &arg: arguments) {
            if (arg->get_type() == LIST) {
                std::string buffer;
                buffer = arg->evaluate();
                auto new_node = std::make_shared<NumberNode>(stoi(buffer));
                arguments.erase(it);
                arguments.insert(it, new_node);
            }
            ++it;
        }

        arguments.insert(arguments.begin(), operation);
        auto eval_node = ListNode(arguments);
        arguments.clear();
        return eval_node.evaluate();
    }

    [[nodiscard]] size_t get_num_args() const {
        return num_args;
    }

    void set_argument(const std::shared_ptr<Node> &arg) {
        arguments.push_back(arg);
    }

    std::string get_operation() {
        return operation->get_value();
    }
};

class MapcarNode : public Node {
private:
    std::shared_ptr<SymbolNode> operation;
    std::shared_ptr<ListNode> list;
public:
    MapcarNode(std::shared_ptr<SymbolNode> o, std::shared_ptr<ListNode> lst) : operation(std::move(o)),
                                                                               list(std::move(lst)) {
        type = MAPCAR;
    }

    std::string evaluate() override {
        std::string buffer;
        std::vector<std::shared_ptr<ListNode>> args;

        for (const auto &lst: list->get_nodes()) {
            auto new_node = std::make_shared<ListNode>();
            new_node->add_node(operation);
            new_node->add_node(lst);
            args.push_back(new_node);
        }

        for (auto &arg: args) {
            buffer += arg->evaluate() + " ";
        }

        return buffer;
    }
};

class ConditionalNode : public Node {
private:
    std::shared_ptr<Node> condition;
    std::shared_ptr<Node> true_branch;
    std::shared_ptr<Node> false_branch;

public:
    ConditionalNode(std::shared_ptr<Node> c, std::shared_ptr<Node> t, std::shared_ptr<Node> f)
            : condition(std::move(c)), true_branch(std::move(t)), false_branch(std::move(f)) {
        type = CONDITIONAL;
    }

    std::string evaluate() override {
        // Evaluate the condition and choose the branch based on its boolean result
        if (condition->evaluate() == "T") {
            return true_branch->evaluate();
        } else {
            return false_branch->evaluate();
        }
    }
};


class ConditionNode : public Node {
private:
    std::string operator_;
    std::shared_ptr<Node> leftOperand;
    std::shared_ptr<Node> rightOperand;

public:
    // Constructor assumes operator is a simple string
    ConditionNode(const std::string& op, std::shared_ptr<Node> left, std::shared_ptr<Node> right)
            : operator_(op), leftOperand(std::move(left)), rightOperand(std::move(right)) {
        type = CONDITIONAL;
    }

    std::string evaluate() override {
        int left = std::stoi(leftOperand->evaluate());
        int right = std::stoi(rightOperand->evaluate());

        if (operator_ == ">") {
            return left > right ? "T" : "NIL";
        } else if (operator_ == "<") {
            return left < right ? "T" : "NIL";
        } else if (operator_ == "=") {
            return left == right ? "T" : "NIL";
        } else if (operator_ == "!=") {
            return left != right ? "T" : "NIL";
        } else if (operator_ == "and") {
            return (left != 0 && right != 0) ? "T" : "NIL";
        } else if (operator_ == "or") {
            return (left != 0 || right != 0) ? "T" : "NIL";
        } else if (operator_ == "not") {
            // Handle 'not' correctly if only one operand should be present
            return (left == 0) ? "T" : "NIL";
        } else {
            throw std::runtime_error("Unsupported operator: " + operator_);
        }
    }
};

class ConsNode : public Node {
private:
    std::shared_ptr<Node> car;
    std::shared_ptr<Node> cdr;

public:
    ConsNode(std::shared_ptr<Node> car, std::shared_ptr<Node> cdr) : car(std::move(car)), cdr(std::move(cdr)) {
        type = CONS;
    }

    std::string evaluate() override {
        std::string car_result = car->evaluate();
        std::string cdr_result = cdr->evaluate();

        cdr_result.pop_back(); // Remove the last space

        // Check if car_result is a list and should be treated as a sublist
        std::shared_ptr<ListNode> check = std::dynamic_pointer_cast<ListNode>(car);
        bool isCarSublist = (check->get_nodes_size() > 1);

        // Determine how to format the output based on the content of cdr_result
        if (cdr_result.empty()) {
            return "(" + car_result + ")";
        } else {
            if (isCarSublist) {
                car_result.pop_back(); // Remove the last space
                return "((" + car_result + ") " + cdr_result + ")";
            } else {
                return "(" + car_result + cdr_result + ")";
            }
        }
    }
};

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
        (*variables)[name] = value->evaluate();
        return name;
    }
};

class SetNode : public Node {
private:
    std::string name;
    std::shared_ptr<Node> statement;
    std::shared_ptr<std::unordered_map<std::string, std::string>> variables;

public:
    SetNode(std::string name, std::shared_ptr<Node> s, std::shared_ptr<std::unordered_map<std::string, std::string>> vars)
            : name(std::move(name)), statement(std::move(s)), variables(std::move(vars)) {
        type = SET;
    }

    std::string evaluate() override {
        if(variables->find(name) == variables->end()) {
            throw std::runtime_error("Variable " + name + " not defined");
        }

        auto value = std::make_shared<NumberNode>(stoi(statement->evaluate()));
        return DefineNode(name, value, variables).evaluate();
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
        return name;
    }
};

class QuoteNode : public Node {
public:
    explicit QuoteNode(const std::vector<Tokens::Token> &tokens) {
        type = QUOTE;
        for (const auto &token : tokens) {
            value += token.get_value() + " ";
        }
    }

    std::string evaluate() override {
        return value;
    }

};

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

class LambdaNode : public Node {
private:
    std::shared_ptr<ListNode> lambda_list;
    std::shared_ptr<ListNode> lambda_body;

public:
    LambdaNode(std::shared_ptr<ListNode> l_list, std::shared_ptr<ListNode> l_body) : lambda_list(std::move(l_list)),
                                                                                     lambda_body(std::move(l_body)) {
        type = LAMBDA;
    }

    std::string evaluate() override {
        std::string buffer;
        std::vector<std::shared_ptr<Node>> args;
        std::vector<std::shared_ptr<Node>> body;

        for (const auto &node: lambda_list->get_nodes()) {
            args.push_back(node);
        }

        for (const auto &node: lambda_body->get_nodes()) {
            body.push_back(node);
        }

        for (const auto &node: body) {
            buffer += node->evaluate() + " ";
        }

        return buffer;
    }
};
#endif //LISPINTERPRETER_NODES_H
