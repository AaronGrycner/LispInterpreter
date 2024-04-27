# README for Lisp Interpreter

## Overview
This document outlines the syntax for various constructs in a custom Lisp interpreter. The interpreter supports a variety of standard Lisp operations and custom functions.

## Constructs and Syntax

### 1. Number
- **Syntax**: Direct use of numerical values. As per instructions, only 32 bit integers.
- **Example**: `123`

### 2. Boolean
- **Syntax**: `T` for true, `NIL` for false.
- **Example**: `T`, `NIL`

### 3. Symbol
- **Syntax**: Any non-reserved keyword.
- **Example**: `x`, `myVar`

### 4. Quote
- **Syntax**: `(quote expression)`
- **Example**: `(quote (1 2 3))` or `'(1 2 3)`

### 5. Conditional (if-else)
- **Syntax**: `(if condition true-branch false-branch)`
- **Example**: `(if (> x 5) 'yes 'no)`

### 6. Define (variable assignment)
- **Syntax**: `(define symbol expression)`
- **Example**: `(define x 10)`

### 7. Set (variable update)
- **Syntax**: `(set symbol expression)`
- **Example**: `(set x 20)`

### 8. Function Definition (defun)
- **Syntax**: `(defun functionName (params) body)`
- **Example**: `(defun square (x) (* x x))`

### 9. Lambda
- **Syntax**: `(lambda (params) body)`
- **Example**: `(lambda (x y) (+ x y))`

### 10. Car (first element of a list)
- **Syntax**: `(car list)`
- **Example**: `(car '(a b c))` returns `a`

### 11. Cdr (rest of the list)
- **Syntax**: `(cdr list)`
- **Example**: `(cdr '(a b c))` returns `(b c)`

### 12. Cons (construct a list)
- **Syntax**: `(cons x list)`
- **Example**: `(cons 'a '(b c))` returns `(a b c)`

### 13. Mapcar (apply function to each element in the list)
- **Syntax**: `(mapcar function list)`
- **Example**: `(mapcar square '(1 2 3 4))` returns `(1 4 9 16)`

### 14. Relations (comparative and logical operations)
- **Syntax**: `(op expr1 expr2)` where `op` could be `=`, `<`, `>`, etc.
- **Example**: `(> x 5)`

## Additional Notes

- The interpreter assumes that each construct is correctly formatted and enclosed within parentheses `()`.
- Symbols are case-sensitive.
- Numbers can be integers or floats.
- Booleans are represented as `T` for true and `NIL` for false.
- Errors in syntax or unmatched parentheses will result in runtime errors.

## Conclusion

This README provides a concise guide to the basic syntax expected by the Lisp interpreter based on the provided implementation. Users should ensure that expressions are correctly formatted according to these guidelines to avoid runtime errors.
