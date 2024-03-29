/**
 * Project: Translator of language IFJ22
 * @file scanner.h
 * @author János László Vasík - xvasik05
 * @author Andrej Smatana - xsmata03
 * 
 * @brief Header file of scanner module
 */

#ifndef SCANNER_H
#define SCANNER_H

#include "str.h"

/** @brief Types of tokens */
typedef enum {
    // single chars
    TOK_LEFT_PAREN,
    TOK_RIGHT_PAREN,
    TOK_LEFT_BRACE,
    TOK_RIGHT_BRACE,
    TOK_SEMICOLON,
    TOK_COLON,
    TOK_COMMA,

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
    TOK_FUNCTION,     // function
    TOK_TYPE_ID,        // ?int, ?float, ?string, stored in token->attribute.kwVal
    TOK_VARIABLE,       // variable

    // literals
    TOK_STRING_LIT,
    TOK_INT_LIT,
    TOK_DEC_LIT,

    // keyword
    TOK_KEYWORD,

    TOK_PROLOGUE,
    TOK_DECLARE_STRICT,
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
} keyword_t;

/** @brief Structure that holds the token properties */
typedef struct {
    tokenType_t type;
    union {
        int intVal;
        double decVal;
        string_t * strVal;
        keyword_t kwVal;
    } attribute;
} token_t;

/** @brief States for FSM */
typedef enum {
    // end states
    S_START,

    // mid-states
    S_STRT_NEG_COMP, // start negative comparison
    S_MID_NEG_COMP, // mid negative comparison
    S_ASSIGN,
    S_STRT_COMP, // start comparison
    S_LESSER,

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
    S_KEYW_OR_ID, // keyword or id
    S_QSTN_MARK,
    S_TYPE_ID,
    S_STRT_VAR, // start variable
    S_SLASH,
    S_S_COMMENT, // single line comment
    S_M_COMMENT_FIN, // multiline comment finish
    S_M_COMMENT, // multiline comment
} machineState_t;

/** 
 * @brief Allocate memory for token
 * 
 * @return pointer to allocated token on success, or NULL pointer
 */
token_t * tokenInit();

/**
 * @brief Frees allocated attributes of token
 * @param token token to operate with
 */
void cleanToken(token_t * token);

/**
 * @brief Frees token and and any allocated attributes
 * @param token token to operate with
 */
void freeToken(token_t * token);

/**
 * @brief Checks if keyword is in token, if yes, changes it's type. Also destroys string_t if it finds a valid keyword.
 * @param token token to operate with
 * @param s string to compare keywords to
 *
 * @return 0 in case of no keyword, otherwise 1
 */
int checkKeyword(token_t * token, string_t * s);

/**
 * @brief Checks if token contains an operator
 * @param token token to operate with
 * 
 * @return 0 if operator is found, otherwise 1
 */
int checkForOperator(token_t * token);

/**
 * @brief Converts string to integer
 * @param s string to be converted
 * @param base base of numeric value to be converted
 * 
 * @return converted integer value
 */
int convertStringToInt(char * s, int base);

/**
 * @brief Converts string to double
 * @param s string to be converted
 * 
 * @return converted double value
 */
double convertStringToDouble(string_t * s);

/**
 * @brief Compares a string to the string on input
 * @param fp pointer to input stream
 * @param toMatch string to match
 * 
 * @return 0 if match is found, otherwise ERR_LEX_ANALYSIS
 */
int checkForMatch(FILE * fp, char * toMatch);

/**
 * @brief Fills string with characters, and determines its type on request
 * @param s string to operate with
 * @param token token to operate with for declaring type, attributes
 * @param fp pointer to input stream
 * @param varFlag flag to change type of token to variable (1) or function (0) ID, any other number doesn't change the type
 * 
 * @return SUCCESS, otherwise ERR_INTERNAL
 */
int fillStr(string_t * s, token_t * token, FILE * fp, int varFlag);

/**
 * @brief Scans token
 * @param token token to write into
 * 
 * @return SUCCESS, otherwise ERR_INTERNAL or ERR_LEX_ANALYSIS
 */ 
int scanToken(token_t * token);

token_t *copyToken(token_t *token);

#endif // SCANNER_H
