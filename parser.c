#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define COLOR_RED     "\033[0;31m"
#define COLOR_GREEN   "\033[0;32m"
#define COLOR_RESET   "\033[0m"

char* input_file;
int current_position = 0;
int current_line = 1;

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
                printf(COLOR_RED"\nNames cannot contain whitespaces or special characters like !, #, %%, etc.\t" COLOR_RESET);
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

