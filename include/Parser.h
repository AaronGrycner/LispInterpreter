//
// Created by aaron on 4/23/24.
//

#ifndef LISPINTERPRETER_PARSER_H
#define LISPINTERPRETER_PARSER_H

#include "include/Token/Token.h"
#include "Nodes/Nodes.h"
#include "Nodes/List.h"
#include "Nodes/Conditional.h"
#include "Nodes/Define.h"

#include <vector>
#include <string>
#include <memory>

class Parser {
private:
    std::shared_ptr<Node>  parse_expression(std::vector<Tokens::Token>::iterator &it, const std::vector<Tokens::Token>::iterator &end);
    std::shared_ptr<ListNode> parse_list(std::vector<Tokens::Token>::iterator &it, const std::vector<Tokens::Token>::iterator &end);
    std::shared_ptr<ConditionalNode> parse_conditional(std::vector<Tokens::Token>::iterator &it, const std::vector<Tokens::Token>::iterator &end);
    std::shared_ptr<Node> parse_condition(std::vector<Tokens::Token>::iterator &it, const std::vector<Tokens::Token>::iterator &end);
    std::shared_ptr<Node> parse_define(std::vector<Tokens::Token>::iterator &it, const std::vector<Tokens::Token>::iterator &end);
    std::shared_ptr<Node> parse_defun(std::vector<Tokens::Token>::iterator &it, const std::vector<Tokens::Token>::iterator &end);
    std::shared_ptr<Node> parse_set(std::vector<Tokens::Token>::iterator &it, const std::vector<Tokens::Token>::iterator &end);
    std::shared_ptr<Node> parse_relation(std::vector<Tokens::Token>::iterator &it, const std::vector<Tokens::Token>::iterator &end);
    std::shared_ptr<Node> parse_car(std::vector<Tokens::Token>::iterator &it, const std::vector<Tokens::Token>::iterator &end);
    std::shared_ptr<Node> parse_cdr(std::vector<Tokens::Token>::iterator &it, const std::vector<Tokens::Token>::iterator &end);
    std::shared_ptr<Node> parse_cons(std::vector<Tokens::Token>::iterator &it, const std::vector<Tokens::Token>::iterator &end);
    std::shared_ptr<ListNode> parse_cons_list(std::vector<Tokens::Token>::iterator &it, const std::vector<Tokens::Token>::iterator &end);
    std::shared_ptr<Node> parse_mapcar(std::vector<Tokens::Token>::iterator &it, const std::vector<Tokens::Token>::iterator &end);
    std::shared_ptr<FunctionNode> parse_lambda(std::vector<Tokens::Token>::iterator &it, const std::vector<Tokens::Token>::iterator &end);



    std::shared_ptr<std::unordered_map<std::string, std::string>> variables;
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<FunctionNode>>> functions;

public:
    explicit Parser(std::shared_ptr<std::unordered_map<std::string, std::string>> variables,
                    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<FunctionNode>>> functions)
                    : variables(std::move(variables)),
                        functions(std::move(functions)) {}

    std::vector<std::shared_ptr<Node>> operator()(const std::string &input);

};


#endif //LISPINTERPRETER_PARSER_H
