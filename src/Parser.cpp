#include <vector>
#include <memory>
#include <stdexcept>
#include "Parser.h"
#include "Token/Tokenizer.h"
#include "Nodes/Atoms.h"

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

    ++it; // Move past the closing parenthesis
    return list;
}

std::vector<Token>::iterator Parser::find_enclosing_parenthesis(std::vector<Token> tokens, std::vector<Token>::iterator it) {
    auto end_it = tokens.end() - 1;

    while (end_it->get_type() != TokenType::CLOSE_PAREN){
        if (end_it == it){
            throw std::runtime_error("No closing parenthesis found");
        }
        --end_it;
    }
    return end_it;
}