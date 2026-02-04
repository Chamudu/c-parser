#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define COLOR_RED     "\033[0;31m"
#define COLOR_GREEN   "\033[0;32m"
#define COLOR_RESET   "\033[0m"

typedef enum {
    // int print
    TOKEN_INT,
    TOKEN_PRINT,

    //  x y z 1 2 3
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,

    // * / - + = 
    TOKEN_ASSIGN,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_MINUS,
    TOKEN_PLUS,
    
    // ; ()
    TOKEN_SEMICOLON, // ";"
    TOKEN_LPAR, // "(""
    TOKEN_RPAR, // ")"

    TOKEN_FILE_END,
    TOKEN_ERROR
} TokenType ;

typedef struct {
    TokenType type;
    char lexeme[256];
    int line;
} Token;

//AST structure
typedef enum {
    NODE_DECLARATION,
    NODE_ASSIGNMENT,
    NODE_PRINT,
    NODE_BINARY_OP,
    NODE_IDENTIFIER,
    NODE_NUMBER
} ASTNodeType ;

typedef struct ASTNode{
    ASTNodeType type;

    union {
        int number_val;
        char var_name[256];

        struct {
            char op;
            struct ASTNode* left;
            struct ASTNode* right;
        } binary_op;

        struct {
            char var_name[256];
            struct ASTNode* expr;
        } declaration;

        struct {
            char var_name[256];
            struct ASTNode* expr;
        } assignment;

        struct ASTNode* print_expr;
    } data;

} ASTNode;

// symbol table
typedef struct {
    char name[256];
    int value;
    int is_declared;
} Variable;

Variable symbol_table[100];

int symbol_count = 0;
char* input_file;
int current_position = 0;
int current_line = 1;
Token current_token;

int find_variable(char* name) {
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_table[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

void declare_variable(char* name){
    if (find_variable(name) >= 0) {
        printf(COLOR_RED "Semantic Error: Variable '%s' already declared"COLOR_RESET, name);
        exit(1);
    }
    else if (find_variable(name) == -1) {
        strcpy(symbol_table[symbol_count].name, name);
        symbol_table[symbol_count].is_declared = 1;
        symbol_count++;
    }
}

void verify_variable(char* name) {
    if (find_variable(name) == -1) {
        printf(COLOR_RED "Semantic Error: Variable '%s' used but not declared"COLOR_RESET, name);
        exit(1);
    }
}

int set_variable(char* name, int value) {
    int location = find_variable(name);
    verify_variable(name);
    symbol_table[location].value = value; 
    return location;
}

const char* get_token_name(TokenType type) {
    switch (type) {
        case TOKEN_INT: return "int";
        case TOKEN_PRINT: return "print";
        case TOKEN_IDENTIFIER: return "identifier";
        case TOKEN_NUMBER: return "number";
        case TOKEN_ASSIGN: return "=";
        case TOKEN_PLUS: return "+";
        case TOKEN_MINUS: return "-";
        case TOKEN_MULTIPLY: return "*";
        case TOKEN_DIVIDE: return "/";
        case TOKEN_SEMICOLON: return ";";
        case TOKEN_LPAR: return "(";
        case TOKEN_RPAR: return ")";
        case TOKEN_FILE_END: return "End of File";
        default: return "Unknown";
    }
}


void readFile() {
    FILE* file = fopen("inputfile.txt", "rb"); // read only binary

    if (file == NULL) {
        printf(COLOR_RED "ERROR: COULD NOT OPEN THE FILE\n" COLOR_RESET);
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    int file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    input_file = malloc(file_size + 1);
    fread(input_file, 1, file_size, file);

    input_file[file_size] = '\0'; // add null terminator at the end of the file

    fclose(file);
}

char peek() {
    if (input_file[current_position] == '\0') {
        return '\0'; // check
    }

    return input_file[current_position];
}

char peekNext() {
    if (input_file[current_position] == '\0') {
        return  '\0';
    }

    int next_position = current_position + 1;

    return input_file[next_position];
}

char getAdvance() { // get the current character and move forward
    char current = input_file[current_position];
    current_position++;
    return current;
}

void skip_white_space() {
    while (isspace(peek())) {
        if (peek() == '\n') current_line++;
        getAdvance();
    }
}

Token read_identifier() {
    Token token;
    token.line = current_line;

    int i = 0;

    while (isalnum(peek()) || peek() == '_') { //read a identifiers
        token.lexeme[i++] = getAdvance();
    }

    token.lexeme[i] = '\0';

    if (strcmp(token.lexeme, "int") == 0) {
        token.type =TOKEN_INT;
    }
    else if (strcmp(token.lexeme, "print") == 0) {
        token.type = TOKEN_PRINT;
    } 
    else {
        token.type = TOKEN_IDENTIFIER; 
    }

    return token;

}

Token read_number() {
    Token token;
    token.line = current_line;
    int is_valid_token = 1;

    int i = 0;
    while (isalnum(peek()) || peek() == '_') { //read a identifiers
        if (!isdigit(peek())) {
            is_valid_token = 0;
        }
        token.lexeme[i++] = getAdvance();
    }

    if(is_valid_token) {
        token.type = TOKEN_NUMBER;
    }
    else {
        token.type = TOKEN_ERROR;
        printf(COLOR_RED"\nNames must begin with a letter or an underscore \t" COLOR_RESET);
    }

    token.lexeme[i] = '\0';

    return token;
}

Token read_single_character() {
    char c = peek();
    Token token;

    token.line = current_line;
    token.lexeme[0] = getAdvance();
    token.lexeme[1] = '\0';

    switch (c) {
        case '+': token.type = TOKEN_PLUS; return token;        
        case '-': token.type = TOKEN_MINUS; return token; 
        case '*': token.type = TOKEN_MULTIPLY; return token;
        case '/': token.type = TOKEN_DIVIDE; return token;
        case ';': token.type = TOKEN_SEMICOLON; return token;
        case '(': token.type = TOKEN_LPAR; return token;
        case ')': token.type = TOKEN_RPAR; return token;
        case '=': token.type = TOKEN_ASSIGN; return token;
 
        default:
            if (!isspace(peekNext())) {
                int i = 1;

                while (!isspace(peek())) { 
                    token.lexeme[i++] = getAdvance();
                }
                token.lexeme[i] = '\0';

                token.type = TOKEN_ERROR;
                printf(COLOR_RED"\nUnrecognized character %c\n" COLOR_RESET, c);
                return token;
            }
            else {
                token.type = TOKEN_ERROR;
                printf(COLOR_RED"\ninvalid charcater\n" COLOR_RESET);
            }
            return token;
    }
}

Token get_next_token() {
    skip_white_space();

    if (peek() == '\0') {
        Token token;
        token.type = TOKEN_FILE_END;
        strcpy(token.lexeme, "");
        token.line = current_line;
        return token;
    }

    if (isalpha(peek()) || peek() == '_') { // indentifier or keyword (print | int | num1)
        return read_identifier();
    }

    if (isdigit(peek())) { // number
        return read_number();
    }

    else {
        return read_single_character();
    }    
    
}

void eat_Token(TokenType expected_type) {
    if (current_token.type == expected_type) {
        current_token = get_next_token();
    } 
    else {
        printf(COLOR_RED "Syntax Error: Expected '%s' but found '%s' on line %d\n" COLOR_RESET, 
            get_token_name(expected_type), current_token.lexeme, current_token.line);
        exit(1);
    }
}

ASTNode* create_ast_node(ASTNodeType type) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) {
        printf(COLOR_RED "Error: Memory allocation failed (at ASTNode)\n" COLOR_RESET);
        exit(1);
    }

    node->type = type;
    return node;
}

ASTNode* parse_expression();

ASTNode* parse_factor() {

    Token token = current_token;

    if (token.type == TOKEN_NUMBER) {
        ASTNode* node = create_ast_node(NODE_NUMBER);
        node->data.number_val = atoi(token.lexeme);
        eat_Token(TOKEN_NUMBER);
        return node;
    }
    else if (token.type == TOKEN_IDENTIFIER) {
        verify_variable(token.lexeme);

        ASTNode* node = create_ast_node(NODE_IDENTIFIER);
        strcpy(node->data.var_name, token.lexeme);
        eat_Token(TOKEN_IDENTIFIER);
        return node;
    }
    else if (token.type == TOKEN_LPAR) {
        eat_Token(TOKEN_LPAR);
        ASTNode* node = parse_expression();
        eat_Token(TOKEN_RPAR);
        return node;
    }
    else {
        printf(COLOR_RED "Syntax Error: Unexpected token '%s' in line %d\n" COLOR_RESET, 
            token.lexeme, token.line);
        exit(1);
    }
}

ASTNode* parse_term() {
    ASTNode* left = parse_factor();
    
    while (current_token.type == TOKEN_MULTIPLY || current_token.type == TOKEN_DIVIDE) {
        ASTNode* node = create_ast_node(NODE_BINARY_OP);
        node->data.binary_op.op = current_token.lexeme[0];
        node->data.binary_op.left = left;

        eat_Token(current_token.type);

        node->data.binary_op.right = parse_factor();

        left = node;
    }

    return left;

}

ASTNode* parse_expression() {
    ASTNode* left = parse_term();

    while (current_token.type == TOKEN_PLUS || current_token.type == TOKEN_MINUS) {
        ASTNode* node = create_ast_node(NODE_BINARY_OP);

        node->data.binary_op.op = current_token.lexeme[0];
        node->data.binary_op.left = left;

        eat_Token(current_token.type);

        node->data.binary_op.right = parse_term();

        left = node;
    }

    return left;
}

ASTNode* parse_print() {

    eat_Token(TOKEN_PRINT);
    eat_Token(TOKEN_LPAR);

    ASTNode* expr = parse_expression();

    eat_Token(TOKEN_RPAR);
    eat_Token(TOKEN_SEMICOLON);

    ASTNode* print = create_ast_node(NODE_PRINT);

    print->data.print_expr = expr;

    return print;
}

ASTNode* parse_assignment() {
    verify_variable(current_token.lexeme);

    ASTNode* node = create_ast_node(NODE_ASSIGNMENT);
    strcpy(node->data.assignment.var_name, current_token.lexeme);
    eat_Token(TOKEN_IDENTIFIER);
    eat_Token(TOKEN_ASSIGN);

    node->data.assignment.expr = parse_expression();
    eat_Token(TOKEN_SEMICOLON);

    return node;    
}

ASTNode* parse_declaration() {
    
    ASTNode* node = create_ast_node(NODE_DECLARATION);
    eat_Token(TOKEN_INT);

    declare_variable(current_token.lexeme);

    strcpy(node->data.declaration.var_name, current_token.lexeme);
    eat_Token(TOKEN_IDENTIFIER);
    eat_Token(TOKEN_ASSIGN);

    node->data.declaration.expr = parse_expression();

    eat_Token(TOKEN_SEMICOLON);

    return node;
}

ASTNode* parse_statement() {
    Token token = current_token;

    if (token.type == TOKEN_INT) {
        return parse_declaration();
    }
    else if (token.type == TOKEN_IDENTIFIER) {
        return parse_assignment();
    }
    else if (token.type == TOKEN_PRINT) {
        return parse_print();
    }
    else {
        printf(COLOR_RED "Syntax Error: Unexpected token '%s' at start of statement on line %d\n" COLOR_RESET, 
            token.lexeme, token.line);
        exit(1);
    }
}

int evaluate_tree(ASTNode* node) {
    if (node == NULL) return 0;

    switch (node->type) {
        case NODE_NUMBER:
            return node->data.number_val;
        
        case NODE_IDENTIFIER:
            int index = find_variable(node->data.var_name);
            return symbol_table[index].value;
        
        case NODE_BINARY_OP:
            int value_left = evaluate_tree(node->data.binary_op.left);
            int value_right = evaluate_tree(node->data.binary_op.right);
            char op = node->data.binary_op.op;

            if (op == '+') return value_left + value_right;
            if (op == '-') return value_left - value_right;
            if (op == '*') return value_left * value_right;
            if (op == '/') {
                if ( value_right == 0) {
                    printf(COLOR_RED "Error: division by 0 on line %d\n" COLOR_RESET, current_line);
                    exit(1);
                }
                else {
                    return value_left / value_right;
                }
            }
            printf("Evaluate binaryOP error\n");
        
        case NODE_ASSIGNMENT:
            int value_assign_right = evaluate_tree(node->data.assignment.expr);
            set_variable(node->data.assignment.var_name, value_assign_right);
            return value_assign_right;
            
        case NODE_DECLARATION:
            int value_decl_right = evaluate_tree(node->data.declaration.expr);
            set_variable(node->data.declaration.var_name, value_decl_right);
            return value_decl_right;
            
        case NODE_PRINT:
            int print = evaluate_tree(node->data.print_expr);
            printf("%d\n", print);
            return 0;
    }

    return 0;
}

void print_indent(int level) {
    for (int i = 0; i < level; i++) printf("|  "); // 2 spaces per level
}

void print_ast(ASTNode* node, int level) {
    if (node == NULL) return;

    print_indent(level);

    switch (node->type) {
        case NODE_NUMBER:
            printf("NUMBER %d\n", node->data.number_val);
            break;
            
        case NODE_IDENTIFIER:
            printf("VAR %s\n", node->data.var_name);
            break;

        case NODE_BINARY_OP:
            printf("BINARY OP (%c)\n", node->data.binary_op.op);
            print_ast(node->data.binary_op.left, level + 1);
            print_ast(node->data.binary_op.right, level + 1);
            break;

        case NODE_DECLARATION:
            printf("DECL (int %s)\n", node->data.declaration.var_name);
            print_ast(node->data.declaration.expr, level + 1);
            break;

        case NODE_ASSIGNMENT:
            printf("ASSIGN (%s)\n", node->data.assignment.var_name);
            print_ast(node->data.assignment.expr, level + 1);
            break;
            
        case NODE_PRINT:
            printf("PRINT\n");
            print_ast(node->data.print_expr, level + 1);
            break;
    }
}

void parse_program() {
    while (current_token.type != TOKEN_FILE_END) {
        ASTNode* stmt = parse_statement();
        // print_ast(stmt, 0);

        evaluate_tree(stmt);
    }
}



/*

# 1. The Program 
<program>         → <statement>*

# 2. Statements
<statement>       → <declaration> | <assignment> | <print_stmt>

<declaration>     → int IDENTIFIER = <expression> ;
<assignment>      → IDENTIFIER = <expression> ;
<print_stmt>      → print ( <expression> ) ;

# 3. Expressions 
<expression>      → <term> { ("+" | "-") <term> }*

# 4. Terms 
<term>            → <factor> { ("*" | "/") <factor> }*

# 5. Factors 
<factor>          → NUMBER | IDENTIFIER | "(" <expression> ")"

*/


/*
int main() {
    readFile();
    
    printf("=== TOKENIZER OUTPUT ===\n\n");
    
    Token token;
    do {
        token = get_next_token();
        
        // Print token information
        printf("Line %d: ", token.line);
        
        switch(token.type) {
            case TOKEN_INT:         printf("KEYWORD       'int'\n"); break;
            case TOKEN_PRINT:       printf("KEYWORD       'print'\n"); break;
            case TOKEN_IDENTIFIER:  printf("IDENTIFIER    '%s'\n", token.lexeme); break;
            case TOKEN_NUMBER:      printf("NUMBER        '%s'\n", token.lexeme); break;
            case TOKEN_ASSIGN:      printf("OPERATOR      '='\n"); break;
            case TOKEN_PLUS:        printf("OPERATOR      '+'\n"); break;
            case TOKEN_MINUS:       printf("OPERATOR      '-'\n"); break;
            case TOKEN_MULTIPLY:    printf("OPERATOR      '*'\n"); break;
            case TOKEN_DIVIDE:      printf("OPERATOR      '/'\n"); break;
            case TOKEN_SEMICOLON:   printf("DELIMITER     ';'\n"); break;
            case TOKEN_LPAR:        printf("DELIMITER     '('\n"); break;
            case TOKEN_RPAR:        printf("DELIMITER     ')'\n"); break;
            case TOKEN_FILE_END:    printf("END OF FILE\n"); break;
            case TOKEN_ERROR:       printf("ERROR         %s\n\n", token.lexeme); break;
            default:                printf("UNKNOWN TOKEN\n"); break;
        }
        
    } while (token.type != TOKEN_FILE_END);
    
    printf("\n=== TOKENIZATION COMPLETE ===\n");
    
    free(input_file);
    return 0;
}
*/

int main() {
    readFile();
    current_token = get_next_token(); 
    parse_program();
    printf(COLOR_GREEN "Parsing Completed Successfully!\n" COLOR_RESET);
    
    free(input_file);
    return 0;
}