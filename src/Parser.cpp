#include <vector>
#include <memory>
#include <stdexcept>
#include "Parser.h"
#include "Tokenizer.h"
#include "Nodes.h"
#include <iostream>

using namespace Tokens;

typedef std::vector<std::shared_ptr<Node>> NodeVec;

NodeVec Parser::operator()(const std::string &input) {
    std::vector<Token> tokens = Tokenizer::tokenize(input);
    std::vector<std::shared_ptr<Node>> nodes;

    auto it = tokens.begin();

    if (it->get_type() == TokenType::QUOTE) {
        tokens.erase(tokens.begin());
        nodes.push_back(std::make_shared<QuoteNode>(tokens));
        return nodes;
    }

    while (it != tokens.end()) {
        nodes.push_back(parse_expression(it, tokens.end()));
    }

    return nodes;
}

std::shared_ptr<Node>
Parser::parse_expression(std::vector<Token>::iterator &it, const std::vector<Token>::iterator &end) {
    if (it == end) {
        throw std::runtime_error("Unexpected end of input");
    }

    Token token = *it++;

    std::shared_ptr<Node> node;
    std::shared_ptr<ListNode> list;

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
            if (variables->find(token.get_value()) != variables->end()) {
                node = std::make_shared<NumberNode>(std::stod(variables->at(token.get_value())));
                break;
            } else if (functions->find(token.get_value()) != functions->end()) {
                auto function = functions->at(token.get_value());
                int counter{};

                ++it; // Move past the open parenthesis
                while (it != end && it->get_type() != TokenType::CLOSE_PAREN) {
                    function->set_argument(parse_expression(it, end));
                    ++counter;
                }

                if (counter != function->get_num_args()) {
                    throw std::runtime_error(
                            "Expected " + std::to_string(function->get_num_args()) + " arguments, got " +
                            std::to_string(counter));
                }

                ++it; // Move past the close parenthesis

                node = function;
                break;
            }
            node = std::make_shared<SymbolNode>(token.get_value());
            break;

        case TokenType::CONDITIONAL:
            node = parse_conditional(it, end);
            break;
        case TokenType::DEFINE:
            node = parse_define(it, end);
            break;
        case TokenType::DEFUN:
            node = parse_defun(it, end);
            break;
        case TokenType::SET:
            node = parse_set(it, end);
            break;
        case TokenType::RELATION:
            node = parse_relation(it, end);
            break;
        case TokenType::CAR:
            node = parse_car(it, end);
            break;
        case TokenType::CDR:
            node = parse_cdr(it, end);
            break;
        case TokenType::CONS:
            node = parse_cons(it, end);
            break;
        case TokenType::MAPCAR:
            node = parse_mapcar(it, end);
            break;
        case TokenType::LAMBDA:
            node = parse_lambda(it, end);
            break;
        default:
            throw std::runtime_error("Unexpected token: " + token.get_value());
    }

    return node;
}

std::shared_ptr<ListNode>
Parser::parse_list(std::vector<Token>::iterator &it, const std::vector<Token>::iterator &end) {
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

std::shared_ptr<ConditionalNode>
Parser::parse_conditional(std::vector<Token>::iterator &it, const std::vector<Token>::iterator &end) {
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
std::shared_ptr<Node>
Parser::parse_condition(std::vector<Token>::iterator &it, const std::vector<Token>::iterator &end) {
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
        throw std::runtime_error("Unexpected end of input after 'define'");
    }

    // First token after 'define' should be the variable name (symbol)
    if (it->get_type() != TokenType::SYMBOL) {
        throw std::runtime_error("Expected a variable name after 'define'");
    }
    std::string varName = it->get_value();
    ++it; // Move past the variable name

    if (it == end) {
        throw std::runtime_error("Incomplete 'define' expression: missing value");
    }

    // Parse the value expression which can be any valid expression
    std::shared_ptr<Node> value = parse_expression(it, end);

    // Assuming closing ')' is consumed by the caller or handling at the list parsing level
    return std::make_shared<DefineNode>(varName, value, variables);
}

std::shared_ptr<Node> Parser::parse_defun(std::vector<Token>::iterator &it, const std::vector<Token>::iterator &end) {
    int args{};
    std::shared_ptr<FunctionNode> f;
    std::shared_ptr<SymbolNode> oper;

    if (it == end) {
        throw std::runtime_error("Unexpected end of input after 'defun'");
    }

    // Now expecting the function name
    if (it == end || it->get_type() != TokenType::SYMBOL) {
        throw std::runtime_error("Expected function name in function definition");
    }
    std::string functionName = it->get_value();

    ++it; // move past function name
    ++it; // move past open parenthesis to the first operator

    // Parse args
    while (it->get_type() != TokenType::CLOSE_PAREN) {
        if (it->get_type() != TokenType::SYMBOL) {
            throw std::runtime_error("Expected symbol in args list");
        }
        ++args;
        ++it;
    }

    ++it;
    ++it; // move past close parenthesis to the operator

    // Parse operator
    if (it == end || it->get_type() != TokenType::SYMBOL) {
        throw std::runtime_error("Expected operator");
    } else {
        oper = std::make_shared<SymbolNode>(it->get_value());
    }

    for (int i = 0; i < args; ++i) {
        ++it;
    }

    ++it;
    ++it;

    f = std::make_shared<FunctionNode>(args, oper);
    return std::make_shared<FunctionDefineNode>(functionName, f, functions);
}

std::shared_ptr<Node> Parser::parse_set(std::vector<Token>::iterator &it, const std::vector<Token>::iterator &end) {
    if (it == end) {
        throw std::runtime_error("Unexpected end of input after 'set'");
    }

    if (it->get_type() != TokenType::SYMBOL) {
        throw std::runtime_error("Expected var name after 'set'");
    }

    std::string varName = it->get_value();

    ++it; // move past the name

    auto arg = parse_expression(it, end);

    return std::make_shared<SetNode>(varName, arg, variables);
}

std::shared_ptr<Node>
Parser::parse_relation(std::vector<Token>::iterator &it, const std::vector<Token>::iterator &end) {
    --it; // Move back to the operator

    if (it == end) {
        throw std::runtime_error("Unexpected end of input after relation operator");
    }

    std::string op = it->get_value();

    if (it == end) {
        throw std::runtime_error("Unexpected end of input after relation operator");
    }

    ++it;

    std::shared_ptr<Node> left = parse_expression(it, end);

    if (it == end) {
        throw std::runtime_error("Unexpected end of input after left operand of relation");
    }

    std::shared_ptr<Node> right = parse_expression(it, end);

    return std::make_shared<RelationNode>(op, left, right);
}

std::shared_ptr<Node> Parser::parse_car(std::vector<Token>::iterator &it, const std::vector<Token>::iterator &end) {
    if (it->get_type() != TokenType::QUOTE) {
        throw std::runtime_error("Syntax Error, expected quote");
    }

    std::shared_ptr<CarNode> car = std::make_shared<CarNode>();

    ++it; // advance past quote
    ++it; // advance past open paren

    while (it->get_type() != TokenType::CLOSE_PAREN) {
        car->add_node(parse_expression(it, end));
    }

    ++it; // advance past close paren

    return car;
}

std::shared_ptr<Node> Parser::parse_cdr(std::vector<Token>::iterator &it, const std::vector<Token>::iterator &end) {
    if (it->get_type() != TokenType::QUOTE) {
        throw std::runtime_error("Syntax Error, expected quote");
    }

    std::shared_ptr<CdrNode> cdr = std::make_shared<CdrNode>();

    ++it; // advance past quote
    ++it; // advance past open paren

    while (it->get_type() != TokenType::CLOSE_PAREN) {
        cdr->add_node(parse_expression(it, end));
    }

    ++it; // advance past close paren

    return cdr;
}

#include <stdexcept>
#include <memory>
#include "Parser.h"  // Assume necessary includes and namespaces

std::shared_ptr<Node> Parser::parse_cons(std::vector<Token>::iterator &it, const std::vector<Token>::iterator &end) {
    if (it == end || it->get_type() != TokenType::QUOTE) {
        throw std::runtime_error("Syntax Error: Expected quote at the beginning of cons expression");
    }

    std::shared_ptr<Node> car = parse_cons_list(it, end);

    if (it->get_type() == TokenType::OPEN_PAREN) {
        return parse_cons_list(it, end);
    }

    if (it == end) {
        throw std::runtime_error("Syntax Error: Unexpected end of input, expected second expression within cons");
    }

    ++it; // advance past quote

    std::shared_ptr<Node> cdr = parse_cons_list(it, end);

    if (it == end || it->get_type() != TokenType::CLOSE_PAREN) {
        throw std::runtime_error("Syntax Error: Expected close parenthesis after cons expressions");
    }

    ++it; // advance past close paren

    return std::make_shared<ConsNode>(car, cdr); // Return a new ConsNode constructed with car and cdr
}

std::shared_ptr<ListNode>
Parser::parse_cons_list(std::vector<Token>::iterator &it, const std::vector<Token>::iterator &end) {
    std::shared_ptr<ListNode> list = std::make_shared<ListNode>();

    while (it->get_type() == TokenType::OPEN_PAREN || it->get_type() == TokenType::QUOTE) {
        ++it;
    }

    while (it != end && it->get_type() != TokenType::CLOSE_PAREN && it->get_type() != TokenType::QUOTE) {
        std::shared_ptr<SymbolNode> sym = std::make_shared<SymbolNode>(it->get_value());
        list->add_node(sym);
        ++it;
    }

    return list;
}

std::shared_ptr<Node> Parser::parse_mapcar(std::vector<Token>::iterator &it, const std::vector<Token>::iterator &end) {
    std::shared_ptr<ListNode> list;
    std::shared_ptr<SymbolNode> oper;

    if (it == end || it->get_type() != TokenType::QUOTE) {
        throw std::runtime_error("Syntax Error: Expected quote at the beginning of mapcar expression");
    }

    ++it; // advance past quote

    oper = std::make_shared<SymbolNode>(it->get_value()); // get the operator

    ++it; // advance past operator
    ++it; // advance past quote

    list = parse_cons_list(it, end);

    ++it;

    return std::shared_ptr<MapcarNode>(std::make_shared<MapcarNode>(oper, list));
}

std::shared_ptr<FunctionNode>
Parser::parse_lambda(std::vector<Token>::iterator &it, const std::vector<Token>::iterator &end) {
    int args{};
    std::shared_ptr<FunctionNode> fn;

    ++it;

    while (it->get_type() != TokenType::CLOSE_PAREN) {
        ++args;
        ++it;
    }
    ++it; // advance past close paren
    ++it; // advance past open paren

    std::shared_ptr<SymbolNode> oper = std::make_shared<SymbolNode>(it->get_value());

    while (it->get_type() != TokenType::CLOSE_PAREN) {
        ++it;
    }

    ++it; // advance past close paren

    fn = std::make_shared<FunctionNode>(args, oper);

    while (it->get_type() != TokenType::CLOSE_PAREN) {
        fn->set_argument(parse_expression(it, end));
    }

    return fn;
}
