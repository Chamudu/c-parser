#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
    FILE* file = fopen("inputfile.txt", "r");

    if (file == NULL) {
        printf("ERROR: COULD NOT OPEN THE FILE\n");
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

    while (isalnum(peek())) { //read a word(letter or digit)
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
    token.type = TOKEN_NUMBER;
    token.line = current_line;

    int i = 0;
    while (isdigit(peek())) { // improve to drop 212av
        token.lexeme[i++] = getAdvance();
    }

    token.lexeme[i] = '\0';

    return token;
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

    if (isalpha(peek())) {

    }

    if (isdigit(peek())) {
    
    }

    switch (peek()) {
        case '+':
        
        case '-':

        case '*':

        case '/':

        default:
            printf("invalid token");
    
    }
    
}




int main() {
    readFile();
}


