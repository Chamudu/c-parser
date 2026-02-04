# C Parser & Interpreter 🚀

A custom-built Compiler Frontend and Interpreter for a subset of the C language.
This project implements the full pipeline from raw source code to execution, including Lexical Analysis, Parsing (AST), Semantic Validation, and Runtime Interpretation.

---

## ✨ Features

### 1. Lexical Analysis (The Lexer)
- Converts raw source code into a stream of **Tokens**.
- Handles Keywords (`int`, `print`), Identifiers, Numbers, and Operators.
- **Robust Error Handling:** Detects invalid characters and malformed identifiers.

### 2. Syntax Analysis (The Parser)
- Implements a **Recursive Descent Parser**.
- builds a hierarchical **Abstract Syntax Tree (AST)**.
- Handles **Operator Precedence** (multiplication `*` happens before addition `+`).
- Validates grammar rules (e.g., missing semicolons `T_SEMICOLON`).

### 3. Semantic Analysis (The Logic Check)
- **Symbol Table:** tracks variable declarations and memory state.
- **Scope Validation:**
    - Prevents using undeclared variables.
    - Prevents re-declaring existing variables.

### 4. Interpreter (The Runtime)
- Traverses the AST recursively to execute logic.
- Performs arithmetic calculations.
- Handles **Runtime Errors** (like Division by Zero).
- Executes `print()` statements to standard output.

---

## 🛠️ Architecture

The project follows the standard compiler pipeline:

```mermaid
graph TD
    A[Source Code] -->|Lexer| B[Tokens]
    B -->|Parser| C[Abstract Syntax Tree (AST)]
    C -->|Semantic Check| D[Validated AST]
    D -->|Interpreter| E[Output / Result]
```

---

## 💻 Tech Stack
*   **Language:** C (Standard C99)
*   **Tools:** GCC Compiler
*   **No External Libraries:** Built completely from scratch (std libs only).

---

## 🚀 How to Run

1.  **Clone the repository**
2.  **Write your C code** in `inputfile.txt`:
    ```c
    int x = 10;
    int y = 5;
    int result = (x + y) * 2;
    print(result);
    ```
3.  **Compile the Parser:**
    ```bash
    gcc parser.c -o parser
    ```
4.  **Run logic:**
    ```bash
    ./parser inputfile.txt
    ```
    *Note: You can pass any filename as an argument.*

---

## 📝 Supported Grammar

```ebnf
<program>         ::= <statement>*
<statement>       ::= <declaration> | <assignment> | <print_stmt>
<declaration>     ::= "int" IDENTIFIER "=" <expression> ";"
<assignment>      ::= IDENTIFIER "=" <expression> ";"
<print_stmt>      ::= "print" "(" <expression> ")" ";"
<expression>      ::= <term> { ("+" | "-") <term> }
<term>            ::= <factor> { ("*" | "/") <factor> }
<factor>          ::= NUMBER | IDENTIFIER | "(" <expression> ")"
```
