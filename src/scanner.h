/**
 * @file scanner.h
 *
 * @brief Scanner interface
 */

#ifndef SCANNER_H
#define SCANNER_H

// php 
typedef enum {
    // single character
    TOK_LEFT_PAREN,     /* ( */
    TOK_RIGHT_PAREN,    /* ) */
    TOK_SEMICOLON,      /* ; */
    TOK_COLON,          /* : */
    TOK_COMMA,          /* , */
    TOK_LEFT_BRACE,     /* { */
    TOK_RIGHT_BRACE,    /* } */
    TOK_DOT,            /* . */
    TOK_MINUS,          /* - */
    TOK_PLUS,           /* + */
    TOK_SLASH,          /* / */
    TOK_BACKSLASH,      /* \ */
    TOK_STAR,           /* * */

    // one or two character
    TOK_BANG,                   /* !   */
    TOK_BANG_EQUAL,             /* !=  */
    TOK_BANG_EQUAL_EQUAL,       /* !== */
    TOK_EQUAL,                  /* =   */
    TOK_EQUAL_EQUAL,            /* ==  */
    TOK_EQUAL_EQUAL_EQUAL,      /* === */
    TOK_GREATER,                /* >   */
    TOK_GREATER_EQUAL,          /* >=  */
    TOK_LESS,                   /* <   */
    TOK_LESS_EQUAL,             /* <=  */

    // literals
    TOK_IDENTIFIER,            /* $foo */
    TOK_STRING,                /* "foo" */
    TOK_NUMBER,                /* 123 */
    
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
    
    TOK_QUESTION_MARK,  // TODO type id?

} TokenType;

#endif /* SCANNER_H */
