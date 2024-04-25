#include <vector>
#include <memory>
#include <stdexcept>
#include "Parser.h"
#include "Token/Tokenizer.h"
#include "Nodes/Atoms.h"
#include "Nodes/Conditional.h"
#include "Nodes/Define.h"
#include <iostream>

using namespace Tokens;

typedef std::vector<std::shared_ptr<Node>> NodeVec;

NodeVec Parser::operator()(const std::string &input) {
    std::vector<Token> tokens = Tokenizer::tokenize(input);
    std::vector<std::shared_ptr<Node>> nodes;

    auto it = tokens.begin();

    while (it != tokens.end()) {
        nodes.push_back(parse_expression(it, tokens.end()));
    }

    return nodes;
}

std::shared_ptr<Node> Parser::parse_expression(std::vector<Token>::iterator &it, const std::vector<Token>::iterator &end) {
    if (it == end) {
        throw std::runtime_error("Unexpected end of input");
    }

    Token token = *it++;

    std::cout << "Token: " << token.get_value() << std::endl;

    std::shared_ptr<Node> node;

    switch (token.get_type()) {
        case TokenType::OPEN_PAREN:
            node = parse_list(it, end);
            break;
        case TokenType::NUMBER:
            node = std::make_shared<NumberNode>(std::stod(token.get_value()));
            break;
        case TokenType::BOOLEAN:
            node = std::make_shared<BooleanNode>(token.get_value() == "T");
            break;
        case TokenType::SYMBOL:
            node = std::make_shared<SymbolNode>(token.get_value());
            break;
        case TokenType::CONDITIONAL:
            node = parse_conditional(it, end);
            break;
        case TokenType::DEFINE:
            node = parse_define(it, end);
            break;
        default:
            throw std::runtime_error("Unexpected token: " + token.get_value());
    }

    return node;
}

std::shared_ptr<ListNode> Parser::parse_list(std::vector<Token>::iterator &it, const std::vector<Token>::iterator &end) {
    std::shared_ptr<ListNode> list = std::make_shared<ListNode>();

    while (it != end && it->get_type() != TokenType::CLOSE_PAREN) {
        list->add_node(parse_expression(it, end));
    }

    if (it == end) {
        throw std::runtime_error("Unexpected end of input");
    }

    ++it;

    return list;
}

std::shared_ptr<ConditionalNode> Parser::parse_conditional(std::vector<Token>::iterator &it, const std::vector<Token>::iterator &end) {
    if (it == end) {
        throw std::runtime_error("Unexpected end of input");
    }

    // No need to check for OPEN_PAREN as 'if' directly leads to the condition in your syntax
    // Parse condition
    std::shared_ptr<Node> condition = parse_condition(it, end);
    if (it == end) throw std::runtime_error("Incomplete conditional expression");

    // Parse consequent branch
    std::shared_ptr<Node> true_branch = parse_expression(it, end);
    if (it == end) throw std::runtime_error("Incomplete conditional expression");

    // Parse alternative branch
    std::shared_ptr<Node> false_branch = parse_expression(it, end);
    if (it == end) throw std::runtime_error("Incomplete conditional expression");

    // Ensure that you're now at the end of this conditional expression, usually a closing parenthesis in Lisp syntax
    if (it->get_type() != TokenType::CLOSE_PAREN) {
        throw std::runtime_error("Expected ')' at the end of conditional");
    }

    return std::make_shared<ConditionalNode>(condition, true_branch, false_branch);
}

// Adjusted parse_condition to handle unary operators
std::shared_ptr<Node> Parser::parse_condition(std::vector<Token>::iterator &it, const std::vector<Token>::iterator &end) {
    if (it == end) throw std::runtime_error("Unexpected end of input while parsing condition");
    if (it->get_type() != TokenType::OPEN_PAREN) throw std::runtime_error("Expected '(' at start of condition");
    ++it;

    if (it == end || it->get_type() != TokenType::SYMBOL)
        throw std::runtime_error("Expected condition operator after '('");
    std::string operator_ = it->get_value();
    ++it;

    std::shared_ptr<Node> leftOperand = parse_expression(it, end);
    if (operator_ == "not") {  // Unary operator
        if (it->get_type() != TokenType::CLOSE_PAREN) throw std::runtime_error("Expected ')' after unary condition");
        ++it;
        return std::make_shared<ConditionNode>(operator_, leftOperand, nullptr);
    }

    // Continue for binary operators
    if (it == end) throw std::runtime_error("Unexpected end of input while parsing first operand of condition");
    std::shared_ptr<Node> rightOperand = parse_expression(it, end);
    if (it == end) throw std::runtime_error("Unexpected end of input while parsing second operand of condition");

    if (it->get_type() != TokenType::CLOSE_PAREN) throw std::runtime_error("Expected ')' at end of condition");

    ++it;

    return std::make_shared<ConditionNode>(operator_, leftOperand, rightOperand);
}

std::shared_ptr<Node> Parser::parse_define(std::vector<Token>::iterator &it, const std::vector<Token>::iterator &end) {
    if (it == end) {
        throw std::runtime_error("Unexpected end of input before define statement");
    }

    // Expecting the next token to be SYMBOL, representing the variable name
    if (it->get_type() != TokenType::SYMBOL) {
        throw std::runtime_error("Expected a variable name after 'define'");
    }
    auto varName = std::make_shared<SymbolNode>(it->get_value());
    ++it; // Move past the variable name

    if (it == end) {
        throw std::runtime_error("Incomplete define expression: missing value");
    }

    // Parse the value expression which can be any valid expression
    auto value = parse_expression(it, end);

    // Expecting the next token to be the closing parenthesis after the expression
    if (it == end || it->get_type() != TokenType::CLOSE_PAREN) {
        throw std::runtime_error("Expected ')' at the end of define expression");
    }
    ++it; // Move past the closing parenthesis

    return std::make_shared<DefineNode>(varName, value, variables);
}



