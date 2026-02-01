# C Parser & Interpreter Project

> **📚 Educational Project**: This is a learning project for understanding compiler construction principles, built as part of my Computational Models coursework. The project demonstrates practical implementation of formal language theory, lexical analysis, parsing, and interpretation.

## 📖 Overview
A complete parser and interpreter for a simple programming language, implementing core compiler concepts from scratch in C.

### Supported Language Features:
- Variable declarations: `int x = 20;`
- Arithmetic expressions: `int z = x + y;`
- Print statements: `print(z);`
- Operators: `+`, `-`, `*`, `/`

## 🎯 Learning Objectives

This project teaches:
- **Formal Language Theory** - Context-Free Grammars
- **Lexical Analysis** - Tokenization and pattern matching
- **Syntax Analysis** - Recursive descent parsing
- **Semantic Analysis** - Symbol tables and type checking
- **Interpretation** - AST evaluation and execution

## 📂 Project Structure

```
c-parser/
├── parser.c                    # Main implementation (to be built)
├── test_inputs/               # Test files
│   ├── valid1.txt
│   ├── valid2.txt
│   └── error1.txt
├── docs/
│   ├── LEARNING_PLAN.md       # Overall project phases
│   ├── PHASE1_CFG_THEORY.md   # Grammar theory & design
│   ├── IMPLEMENTATION_ROADMAP.md  # Technical architecture
│   └── NOTION_STUDY_GUIDE.md  # Comprehensive study notes
└── README.md                   # This file
```

## 🚀 Quick Start

### Compilation
You'll compile with:
```bash
gcc parser.c -o parser
```

### Usage
```bash
./parser inputfile.txt
```

### Example Input (`inputfile.txt`):
```c
int y = 5;
int x = 20;
int z = x + y;
print(z);
```

### Expected Output:
```
25
```

## 📚 Documentation

### For Learning:
1. **Start here:** [`LEARNING_PLAN.md`](LEARNING_PLAN.md)
   - Overview of all phases
   - Timeline and milestones
   
2. **Theory deep-dive:** [`PHASE1_CFG_THEORY.md`](PHASE1_CFG_THEORY.md)
   - Context-Free Grammar design
   - Left recursion and operator precedence
   - Manual parsing examples
   
3. **Implementation guide:** [`IMPLEMENTATION_ROADMAP.md`](IMPLEMENTATION_ROADMAP.md)
   - Data structures
   - Code architecture
   - Testing strategy
   
4. **Study notes:** [`NOTION_STUDY_GUIDE.md`](NOTION_STUDY_GUIDE.md)
   - Comprehensive theory notes
   - Viva preparation
   - Real-world applications

## 📝 Grammar (CFG)

Our language is defined by the following Context-Free Grammar:

```ebnf
<program>         → <statement>+

<statement>       → <declaration> | <print-statement>

<declaration>     → int IDENTIFIER = <expression> ;

<print-statement> → print ( <expression> ) ;

<expression>      → <term> ( (+ | -) <term> )*

<term>            → <factor> ( (* | /) <factor> )*

<factor>          → NUMBER | IDENTIFIER | ( <expression> )
```

**Terminals:**
- Keywords: `int`, `print`
- Operators: `=`, `+`, `-`, `*`, `/`
- Delimiters: `;`, `(`, `)`
- Literals: `NUMBER`, `IDENTIFIER`

## 🏗️ Architecture

```
Source Code → Lexer → Parser → Semantic Analyzer → Executor → Output
              (Tokens)  (AST)    (Validated AST)   (Evaluation)
```

### Components:

1. **Lexer (Tokenizer)**
   - Reads source character-by-character
   - Produces token stream
   - Recognizes keywords, operators, identifiers, numbers

2. **Parser**
   - Recursive descent implementation
   - Builds Abstract Syntax Tree (AST)
   - Validates syntax against CFG

3. **Semantic Analyzer**
   - Symbol table management
   - Checks for undeclared/redeclared variables
   - Type validation

4. **Executor (Interpreter)**
   - Evaluates AST
   - Performs arithmetic operations
   - Outputs results

## 🧪 Testing

### Test Categories:

**Valid Programs:**
- Basic arithmetic
- Operator precedence
- Parenthesized expressions
- Multiple variables

**Syntax Errors:**
- Missing semicolons
- Invalid token sequences
- Unmatched parentheses

**Semantic Errors:**
- Undeclared variables
- Redeclared variables
- Type mismatches

## 🎯 Key Learning Outcomes

### Technical Skills Demonstrated:
- Context-Free Grammar design and implementation
- Handling left recursion and operator precedence
- Symbol table management strategies
- Comprehensive error handling (syntax & semantic)
- Memory management in C
- Modular code architecture

### Implementation Highlights:
- Clean, well-documented code structure
- Recursive descent parsing algorithm
- AST-based expression evaluation
- Extensive test coverage

## 📖 References

### Books:
- "Compilers: Principles, Techniques, and Tools" (Dragon Book)
- "Crafting Interpreters" by Robert Nystrom

### Online Resources:
- Stanford CS143 Compiler Course
- Computerphile YouTube Channel
- LLVM Documentation

## ✅ Project Milestones

- [x] Define Context-Free Grammar
- [x] Design token types and structures
- [ ] Implement lexical analyzer (tokenizer)
- [ ] Build recursive descent parser
- [ ] Add syntax error detection
- [ ] Implement semantic analysis (symbol table)
- [ ] Add semantic error detection
- [ ] Implement expression evaluation
- [ ] Add interpreter functionality
- [ ] Create comprehensive test suite
- [ ] Document design decisions

## 💡 Extension Ideas

If time permits, consider adding:
- More operators (`%`, `^`, `==`, `!=`)
- Multiple data types (`float`, `char`)
- Control flow (`if`, `while`)
- Functions
- Arrays

## 🤝 Development Approach

This project follows a **structured learning methodology**:
- Theory-driven implementation (understand concepts before coding)
- Incremental development with clear milestones
- Test-driven approach for reliability
- Comprehensive documentation alongside code
- Focus on deep understanding, not just working code

The implementation prioritizes clarity and educational value while maintaining professional coding standards.

---

**Goal:** Build a solid foundation in compiler construction that demonstrates both theoretical knowledge and practical implementation skills - essential for any computer science professional. 🚀
