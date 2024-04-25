//
// Created by aaron on 4/23/24.
//

#ifndef LISPINTERPRETER_PARSER_H
#define LISPINTERPRETER_PARSER_H

#include "include/Token/Token.h"
#include "Nodes/Nodes.h"
#include "Nodes/List.h"
#include "Nodes/Conditional.h"

#include <vector>
#include <string>
#include <memory>

class Parser {
private:
    static std::shared_ptr<Node>  parse_expression(std::vector<Tokens::Token>::iterator &it, const std::vector<Tokens::Token>::iterator &end);
    static std::shared_ptr<ListNode> parse_list(std::vector<Tokens::Token>::iterator &it, const std::vector<Tokens::Token>::iterator &end);
    static std::shared_ptr<ConditionalNode> parse_conditional(std::vector<Tokens::Token>::iterator &it, const std::vector<Tokens::Token>::iterator &end);
    static std::shared_ptr<Node> parse_condition(std::vector<Tokens::Token>::iterator &it, const std::vector<Tokens::Token>::iterator &end);

public:
    Parser()=default;
    std::vector<std::shared_ptr<Node>> operator()(const std::string &input);

};


#endif //LISPINTERPRETER_PARSER_H
