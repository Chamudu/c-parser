#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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