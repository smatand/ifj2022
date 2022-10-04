/**
 * @file scanner.h
 *
 * @brief Scanner interface
 */

#ifndef SCANNER_H
#define SCANNER_H

#include <stdbool.h>

// php 
typedef enum {
    // single characters
    TOK_LEFT_PAREN,     /* ( */
    TOK_RIGHT_PAREN,    /* ) */
    TOK_SEMICOLON,      /* ; */
    TOK_COLON,          /* : */
    TOK_COMMA,          /* , */
    TOK_LEFT_BRACE,     /* { */
    TOK_RIGHT_BRACE,    /* } */

    // operators
    TOK_DOT,                    /* . */
    TOK_MINUS,                  /* - */
    TOK_PLUS,                   /* + */
    TOK_SLASH,                  /* / */
    TOK_STAR,                   /* * */
    TOK_NEG_COMPARISON,         /* !== */
    TOK_ASSIGN,                 /* =   */
    TOK_COMPARISON,             /* === */
    TOK_GREATER,                /* >   */
    TOK_GREATER_EQUAL,          /* >=  */
    TOK_LESS,                   /* <   */
    TOK_LESS_EQUAL,             /* <=  */

    // identifiers
    TOK_VAR_ID,                 /* $foo */
    TOK_TYPE_ID,                /* ?foo */
    TOK_FUN_ID,                 /* built-in or user made function IDs */
    
    // literals
    TOK_STRING_LIT,             /* "foo" */
    TOK_INT_LIT,                /* 123 */
    TOK_DEC_LIT,                /* 123.123 */
    TOK_EXP_LIT,                /* integer or decimal with an exponent */
    
    // keywords
    TOK_IF,                    /* if */
    TOK_ELSE,                  /* else */
    TOK_WHILE,                 /* while */
    TOK_FUNCTION,              /* function */
    TOK_RETURN,                /* return */
    TOK_TRUE,                  /* true */
    TOK_FALSE,                 /* false */
    TOK_NULL,                  /* null */
    TOK_INT,                   /* int */
    TOK_FLOAT,                 /* float */

    // prologue TODO is it okay to have this here?
    TOK_PROLOGUE,               /* <?php */
    TOK_END_SIGN,               /* ?> */

} TokenType;

typedef enum {
    S_START,
    S_L_PARENTH,
    S_R_PARENTH,
    S_SEMICOLON,
    S_COLON,
    S_COMA,
    S_L_BRACE,
    S_R_BRACE,
    S_STRT_NEG_COMP,
    S_MID_NEG_COMP,
    S_ASSIGN,
    S_STRT_COMP,
    S_GREATER,
    S_LESSER,
    S_ADDITION,
    S_SUBTRACT,
    S_MULTIPLY,
    S_CONCAT,
    S_INT_LIT,
    S_STRT_EXP,
    S_MID_EXP,
    S_EXP_LIT,
    S_STRT_DEC,
    S_DEC_LIT,
    S_STR_LIT,
    S_KEYW_OR_ID,
    S_QSTN_MARK,
    TYPE_ID,
    S_END_SIGN,
    S_STRT_VAR,
    S_VAR_ID,
    S_SLASH,
    S_S_COMMENT,
    S_M_COMMENT,
    S_EOL_COUNT,
    S_EOL,
    S_EOF,

} MachineState;

typedef struct String *string_t;

typedef struct Token *token_t;

typedef struct Scanner *scanner_t;

scanner_t scanner_init(FILE *stream);

token_t token_init(scanner_t scanner);

string_t string_init();

bool resize(string_t currString);

bool charPushBack(string_t currString, int c);

#endif /* SCANNER_H */
