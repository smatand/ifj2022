#ifndef SCANNER_H
#define SCANNER_H

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
    } attribute;
} token_t;

/** @brief States for FSM */
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
} machineState_t

/** 
 * @brief Allocate memory for token
 * @param token token to operate with
 * 
 * @return SUCCESS, otherwise ERR_INTERNAL
 */
int tokenInit(token_t * token);

/**
 * @brief Frees token
 * @param token token to operate with
 */
void freeToken(token_t * token);

/**
 * @brief Scans token
 * @param token for return
 * 
 * @return SUCCESS, otherwise ERR_CODE
 */ 
int scanToken(token_t * token);

#endif // SCANNER_H