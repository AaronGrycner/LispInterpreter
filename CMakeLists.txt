cmake_minimum_required(VERSION 3.28)
project(LispInterpreter)

set(CMAKE_CXX_STANDARD 17)


add_executable(LispInterpreter main.cpp
        include/Nodes/Nodes.h
        src/Parser.cpp
        include/Parser.h
        include/Token/Token.h
        src/Token/Tokenizer.cpp
        include/Token/Tokenizer.h
        include/Nodes/Atoms.h
        src/Nodes/List.cpp
        include/Nodes/List.h
        src/Symbols/SymbolTable.cpp
        include/Symbols/SymbolTable.h
        src/Evaluator.cpp
        include/Evaluator.h
        include/Nodes/Conditional.h
        include/Nodes/Define.h
)

target_include_directories(LispInterpreter PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        include
        include/Lists
        include/Lists/Atoms)