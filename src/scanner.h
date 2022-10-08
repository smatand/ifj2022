#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include <stdlib.h>

/** @brief Types of tokens */
typedef enum {
    // initial type
    TOK_EMPTY,

    // single chars
    TOK_LEFT_PAREN,
    TOK_RIGHT_PAREN,
    TOK_LEFT_BRACE,
    TOK_RIGHT_BRACE,
    TOK_SEMICOLON,
    TOK_COLON,
    TOK_COMMMA,

    // operators
    TOK_DOT,
    TOK_MINUS,
    TOK_PLUS,
    TOK_SLASH,
    TOK_STAR,
    TOK_NEG_COMPARISON,
    TOK_ASSIGN,
    TOK_COMPARISON,
    TOK_GREATER,
    TOK_GREATER_EQUAL,
    TOK_LESS,
    TOK_LESS_EQUAL,

    // identifiers
    TOK_IDENTIFIER,     // $foo
    TOK_TYPE_ID,        // ?foo
    TOK_FUN_ID,         // built-in or user made function IDs

    // literals
    TOK_STRING_LIT,
    TOK_INT_LIT,
    TOK_DEC_LIT,

    // keyword
    TOK_KEYWORD

    TOK_PROLOGUE,
    //TOK_KEY_ID
    TOK_END_PROLOGUE,

    TOK_EOF
} tokenType_t;

/** @brief Keywords */
typedef enum {
    KW_IF,
    KW_ELSE,
    KW_INT,
    KW_FLOAT,
    KW_FUNCTION,
    KW_NULL,
    KW_RETURN,
    KW_STRING,
    KW_VOID,
    KW_WHILE,
    KW_TRUE,
    KW_FALSE,

    KW_PHP
} keyword_t;

/** @brief Structure that holds the token properties */
typedef struct {
    tokenType_t type;
    union {
        int intVal;
        double decVal;
        char * strVal;
        keyword_t kwVal;
    }
} token_t;

#endif // SCANNER_H