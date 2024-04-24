//
// Created by aaron on 4/23/24.
//

#ifndef LISPINTERPRETER_SYMBOLTABLE_H
#define LISPINTERPRETER_SYMBOLTABLE_H

#include <unordered_map>
#include <string>
#include <variant>
#include <functional>

struct SymbolTableEntry {
    std::string type; // Could be "variable", "function", "builtin"
    std::variant<int, double, std::string, std::function<void()>> value; // C++17 variant for value flexibility
    // Add more fields as necessary, e.g., function parameters, return type
};


class SymbolTable {
private:
    std::unordered_map<std::string, SymbolTableEntry> table;

public:


};


#endif //LISPINTERPRETER_SYMBOLTABLE_H
