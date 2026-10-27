# C# Compiler from Scratch

An educational project focused on understanding how compilers work internally by implementing a simplified C# compiler in **C17**.

> **AI Disclosure:** This project was developed with the assistance of OpenAI's ChatGPT. AI was used for code generation, explanations, debugging, and architectural guidance.

## Project Goals

The main purpose of this project is to study compiler engineering from the ground up.

The project covers:

1. Lexical Analysis (Lexer)
2. Syntax Analysis (Parser)
3. Abstract Syntax Tree (AST)
4. Semantic Analysis
5. Type Checking
6. Intermediate Representation (IR)
7. Code Generation
8. Runtime and Memory Management

The goal is not to replace Microsoft's C# compiler, but to understand the fundamental principles behind compiler construction.

## Technologies

- **Language:** C17
- **Compiler:** Clang
- **Development Environment:** Visual Studio Code
- **Platform:** macOS
- **Version Control:** Git and GitHub

## Project Structure

```text
csharp-compiler/
├── include/
│   ├── lexer.h
│   └── token.h
├── src/
│   ├── lexer.c
│   └── main.c
├── .gitignore
└── README.md
```

## Current Version — v0.1.0

### Lexer

The first implemented component is a lexical analyzer.

Supported features:

- Basic C# keywords (`int`, `bool`, `string`, `if`, `else`, `while`, `return`)
- Identifiers
- Integer literals
- String literals
- Arithmetic operators
- Comparison operators
- Assignment operator
- Parentheses, braces, and semicolons
- Single-line comments (`//`)
- Multi-line comments (`/* ... */`)
- Source line and column tracking
- Basic lexical error diagnostics

### Example Input

```csharp
int age = 21;
string name = "Pavlo";

if (age >= 18) {
    age = age + 1;
}
```

### Example Lexer Output

```text
TOKEN_INT
TOKEN_IDENTIFIER
TOKEN_EQUAL
TOKEN_NUMBER
TOKEN_SEMICOLON
TOKEN_STRING
TOKEN_IDENTIFIER
TOKEN_EQUAL
TOKEN_STRING_LITERAL
TOKEN_SEMICOLON
...
TOKEN_EOF
```

## Build and Run

### Requirements

A C compiler supporting the C17 standard.

### Compile

```bash
clang -std=c17 -Wall -Wextra -Wpedantic \
    -Iinclude src/main.c src/lexer.c \
    -o compiler
```

### Run

```bash
./compiler
```

Currently, the input C# source code is defined directly in `src/main.c`.

## Roadmap

- [x] Basic Lexer
- [x] Token recognition
- [x] Comments and string literals
- [x] Lexical error diagnostics
- [ ] Parser
- [ ] Abstract Syntax Tree (AST)
- [ ] Semantic Analyzer
- [ ] Type Checker
- [ ] Intermediate Representation
- [ ] Code Generation
- [ ] Runtime
- [ ] Advanced C# features

## AI Assistance

This project is created for educational purposes with the assistance of **OpenAI ChatGPT**.

ChatGPT contributes to:

- Explaining compiler theory and architecture
- Generating example implementations
- Debugging C code
- Explaining algorithms and data structures
- Guiding incremental development

The generated code is reviewed, compiled, tested manually, and studied as part of the learning process.

## Disclaimer

This project is an experimental educational compiler.

It is not affiliated with Microsoft, is not based on Roslyn, and does not currently support the full C# language specification or the .NET runtime.

## License

No license has been selected yet.