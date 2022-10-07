/**
 * @file scanner.h
 *
 * @brief Scanner interface
 */

#ifndef SCANNER_H
#define SCANNER_H

#include <stdbool.h>
#include <stdio.h>

// php 
typedef enum {
    // initial type
    TOK_EMPTY,

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
    TOK_ELSE,                  /* else */
    TOK_FLOAT,                 /* float */
    TOK_FUNCTION,              /* function */
    TOK_IF,                    /* if */
    TOK_INT,                   /* int */
    TOK_NULL,                  /* null */
    TOK_RETURN,                /* return */
    TOK_STRING,                /* string */
    TOK_VOID,                  /* void */
    TOK_WHILE,                 /* while */
    TOK_TRUE,                  /* true */
    TOK_FALSE,                 /* false */

    TOK_PROLOGUE,               /* <? */
    TOK_END_SIGN,               /* ?> */

} TokenType;

typedef enum {
    // end states
    S_START,
    S_L_PARENTH, // left parenthesis
    S_R_PARENTH, // right parenthesis
    S_SEMICOLON, 
    S_COLON,
    S_COMA,
    S_L_BRACE, // left curly brackets
    S_R_BRACE, // right curly brackets
    S_NEG_COMP, // negative comparison
    S_COMP, // comparison
    S_GREATER_EQ, // greater or equals
    S_LESSER_EQ, // lesser or equals
    S_PROLOGUE,
    S_ADDITION,
    S_SUBTRACT,
    S_MULTIPLY,
    S_CONCAT,
    S_STR_LIT, // string literal
    S_ERROR,
    S_END,

    // mid-states
    S_STRT_NEG_COMP, // start negative comparison
    S_MID_NEG_COMP, // mid negative comparison
    S_ASSIGN,
    S_STRT_COMP, // start comparison
    S_LESSER,
    S_GREATER,
    S_INT_LIT, // integer literal
    S_STRT_EXP, // start exponent
    S_MID_EXP, // mid exponent
    S_EXP_LIT, // exponent literal
    S_STRT_DEC, // start decimal
    S_DEC_LIT, // decimal literal
    S_STRT_STR, // start string
    S_STRT_ESCP_SQNC, // start escape sequence
    S_HEX_SCP_SQNC, // hexadecimal escape sequence
    S_OCT_SCP_SQNC, // octal escape sequence
    S_SNGL_SCP_SQNC, // single char escape sequence
    S_KEYW_OR_ID, // keyword or id
    S_QSTN_MARK,
    S_TYPE_ID,
    S_END_SIGN,
    S_STRT_VAR, // start variable
    S_VAR_ID, // variable ID
    S_SLASH,
    S_S_COMMENT, // single line comment
    S_STRT_M_COMMENT, // start multiline comment
    S_M_COMMENT_FIN, // multiline comment finish
    S_M_COMMENT, // multiline comment
    S_EOL_COUNT,
    S_EOL,
    S_EOF,

} MachineState;

/** @brief Structure that holds strings(data), their allocated memory size and current character count. */
typedef struct {
    char *data;
    size_t currLen;
    size_t memSize; // old size of the string
} string_t;

/** @brief Structure that holds strings(data) that make the given token, token types, and the line they're on. */
typedef struct {
    TokenType type;
    string_t * string;
    int line;

    // just one of the following will be used
    union {
        int intVal;
        double decVal;
        char * strVal;
    } attribute;
} token_t;

/** @brief Structure that holds the current token, current state of the FSM, source stream and the current line's number. */
typedef struct {
    token_t * token;
    MachineState state;
    FILE *stream;
    int currLine;
} scanner_t;


/** @brief Structure that holds a token, and the pointer to the next node. */
typedef struct TokenNode token_node_t;

struct TokenNode{
    token_t token;
    token_node_t * next;
};

/** @brief Structure that holds a the first and last node of the list (head and tail). */
typedef struct {
    token_node_t * head;
    token_node_t * tail;
} token_list_t;

//typedef struct String *string_t;
//typedef struct Token *token_t;
//typedef struct Scanner *scanner_t;
//typedef struct TK_node *tk_node_t;
//typedef struct TK_list *tk_list_t;

/**
 * @brief Allocates and initializes a scanner structure.
 * @param stream input stream
 * @return Initialized scanner struct, or NULL on failure.
 */
scanner_t * scanner_init(FILE *stream);

/**
 * @brief Allocates and initializes a token structure in a scanner structure.
 * @param scanner target scanner struct 
 * @return Initialized token struct, or NULL on failure.
 */
token_t * token_init(scanner_t * scanner);

/**
 * @brief Allocates and initializes a string structure, and memory for the data in it.
 * @return Initialized string struct, or NULL on failure.
 */
string_t * string_init();

/**
 * @brief Allocates and initializes a TK_list structure.
 * @return Initialized TK_list structure, or NULL on failure.
 */
token_list_t * list_init();

/**
 * @brief Allocates and initializes a TK_node structure.
 * @return Initialized TK_node structure, or NULL on failure.
 */
token_node_t * node_init();

/**
 * @brief State changer of the final state machine.
 * @param currState current state of the machine
 * @param c character from the next lexeme
 */
MachineState transition(MachineState currState, int c);

/**
 * @brief If the current allocated memory for data in a string structure is not enough, reallocates.
 * @param currString current string 
 * @return Reallocated memory for data, or NULL on failure.
 */
bool resize(string_t * currString);

/**
 * @brief Takes a character from the stream, and omits it back.
 * @param scanner target scanner struct
 * @return First character from stream (int c).
 */
int getChar(scanner_t * scanner);

/**
 * @brief Puts a character into a string struct's data.
 * @param currString current string
 * @param c character to be input
 * @return True on success, false on resize error.
 */
bool charPushBack(string_t * currString, int c);

/**
 * @brief Scans a lexeme from the stream.
 * @param stream input stream
 * @return 0 on success, 1 on lexical error.
 */
int scan(FILE *stream);

#endif /* SCANNER_H */
