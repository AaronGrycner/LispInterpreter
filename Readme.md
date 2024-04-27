README for LISP Interpreter

Overview:
This document provides the syntax and expected behavior of various LISP constructs as implemented in the LISP interpreter project. The interpreter is capable of handling variable references, constants, quotations, conditionals, definitions, and several predefined functions and operators.

Supported Constructs:

1. Variable Reference
    - Syntax: var
    - Example: x
    - Example Output: Returns the value of the variable 'x'.

2. Constant Literals (Numbers & Booleans)
    - Syntax: number | boolean
    - Examples: 42, T, NIL
    - Example Output: 42 → 42, T → T, NIL → NIL

3. Quotation
    - Syntax: 'expression
    - Example: '(1 2 3)
    - Example Output: '(1 2 3) → (1 2 3)

4. Conditional *
    - Syntax: (if test consequence alternative)
    - Example: (if (> x 5) 'high 'low)
    - Example Output: Evaluates 'high' if x > 5, otherwise 'low'.

5. Variable Definition
    - Syntax: (define var expression)
    - Example: (define r 10)
    - Example Output: Variable 'r' is set to 10.

6. Function Call
    - Syntax: (func arg1 arg2 ...)
    - Example: (sqrt 16)
    - Example Output: sqrt(16) → 4.0

7. Assignment
    - Syntax: (set! var expression)
    - NOTE: The variable must be defined before assignment.
    - Example: (set! x 20)
    - Example Output: Variable 'x' is set to 20.

8. Function Definition
    - Syntax: (defun name (arg1 arg2 ...) body)
    - Example: (defun add (x y) (+ x y))
    - Example Output: Defines function 'add'. Usage: (add 5 3) → 8

Predefined Operators and Functions:

1. Arithmetic Operators: +, -, *, /
    - Examples:
        - (+ 1 2) → 3
        - (* 3 4) → 12

2. List Manipulation Functions: car, cdr, cons
    - Examples:
        - (car '(1 2 3)) → 1
        - (cdr '(1 2 3)) → (2 3)
        - (cons '1 '(2 3)) → (1 2 3)

3. Mathematical Functions: sqrt, pow
    - Examples:
        - (sqrt 16) → 4
        - (pow 2 3) → 8

4. Logical Operators: >, <, =, !=, and, or, not
    - Examples:
        - (> 3 2) → T
        - (and T NIL) → NIL

Additional Features:
- The interpreter supports the mapcar and lambda functions as described in the assignment for extra credit.
- mapcar Example: (mapcar 'sqrt '(4 9 16)) → (2 3 4)
- lambda Example: ((lambda (x) (+ x 1)) 99) → 100

Usage:
- Launch the interpreter and type in LISP commands following the syntax rules specified above.
- The interpreter will evaluate the expressions and return the results directly or output them to a specified file.
