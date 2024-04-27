cmake_minimum_required(VERSION 3.28)
project(LispInterpreter)

set(CMAKE_CXX_STANDARD 17)


add_executable(LispInterpreter main.cpp
        include/Nodes.h
        src/Parser.cpp
        include/Parser.h
        include/Token.h
        src/Tokenizer.cpp
        include/Tokenizer.h
        src/Nodes.cpp
        src/Evaluator.cpp
        include/Evaluator.h
        src/Nodes.cpp
        src/Tokenizer.cpp
)

target_include_directories(LispInterpreter PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        include)