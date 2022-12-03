/**
 * @file scanner.c
 * @author János László Vasík, Andrej Smatana
 *
 * @brief Implementation of scanner module for IFJ22
 */

#include "scanner.h"
#include "error.h"
#include "str.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h> // strcmp()
#include <ctype.h>  // isalpha()

#define MAX_KEYWORD_CHARS 9 // 8 + 1 for /0 (function + '\0')

token_t *tokenInit()
{
    token_t *token = malloc(sizeof(token_t));

    return token;
}

void freeToken(token_t *token)
{
    if(token->type == TOK_FUNCTION || token->type == TOK_VARIABLE || token->type == TOK_STRING_LIT)
    {
        stringDestroy(token->attribute.strVal); // if there is a string allocated in token, free it
    }
}

int checkKeyword(token_t *token, string_t *s)
{
    if (!strcmp(s->str, "if"))
    {
        token->attribute.kwVal = KW_IF;
    }
    else if (!strcmp(s->str, "else"))
    {
        token->attribute.kwVal = KW_ELSE;
    }
    else if (!strcmp(s->str, "int"))
    {
        token->attribute.kwVal = KW_INT;
    }
    else if (!strcmp(s->str, "float"))
    {
        token->attribute.kwVal = KW_FLOAT;
    }
    else if (!strcmp(s->str, "function"))
    {
        token->attribute.kwVal = KW_FUNCTION;
    }
    else if (!strcmp(s->str, "null"))
    {
        token->attribute.kwVal = KW_NULL;
    }
    else if (!strcmp(s->str, "return"))
    {
        token->attribute.kwVal = KW_RETURN;
    }
    else if (!strcmp(s->str, "string"))
    {
        token->attribute.kwVal = KW_STRING;
    }
    else if (!strcmp(s->str, "void"))
    {
        token->attribute.kwVal = KW_VOID;
    }
    else if (!strcmp(s->str, "while"))
    {
        token->attribute.kwVal = KW_WHILE;
    }
    else if (!strcmp(s->str, "true"))
    {
        token->attribute.kwVal = KW_TRUE;
    }
    else if (!strcmp(s->str, "false"))
    {
        token->attribute.kwVal = KW_FALSE;
    }
    else
    {
        return 0; // no keyword found, it is ID, need string
    }

    stringDestroy(s);
    token->type = TOK_KEYWORD;
    return 1; // caller must take care of //()!
}

int checkForOperator(token_t * token)
{
    if(token->type == TOK_DOT || token->type == TOK_MINUS || token->type == TOK_PLUS ||
        token->type == TOK_SLASH || token->type == TOK_STAR )
    {
        return 0;
    }
    return 1;
}

int convertStringToInt(char *s, int base)
{
    char *endPtr;

    int res = strtol(s, &endPtr, base);

    return res;
}

double convertStringToDouble(string_t *s)
{
    char *endPtr;

    double res = strtod(s->str, &endPtr);

    return res;
}

int checkForMatch(FILE * fp, char * toMatch) {
    int len = strlen(toMatch);
    char * loadedChars = calloc(len, 1); // 1 is sizeof(char)

    int c = 0;
    for (int i = 0; i < len; i++) {
        c = getc(fp);

        if (c != EOF) {
            loadedChars[i] = c;
        } else {
            return ERR_LEX_ANALYSIS;
        }
    }

    if (strcmp(loadedChars, toMatch)) {
        return ERR_LEX_ANALYSIS;
    } else {
        // match found
        return 0;
    }

}

int fillStr(string_t *s, token_t *token, FILE *fp, int varFlag)
{
    int c = getc(fp);

    while (isalnum(c) || c == '_')
    {
        if (charPushBack(s, c) == ERR_INTERNAL)
        {
            return ERR_INTERNAL;
        }
        c = getc(fp);
    }

    ungetc(c, fp);

    if (varFlag == 1) 
    {
        token->type = TOK_VARIABLE;
    }
    else if (varFlag == 0)
    {
        token->type = TOK_FUNCTION;

        // specifically handling declare function, which is a part of prologue
        if (!strcmp(s->str, "declare")) {
            if (checkForMatch(fp, "(strict_types=1)") == 0) {
                token->type = TOK_DECLARE_STRICT; // declare(strict_types=1) is found
                stringDestroy(s);
                return SUCCESS;
            }
        }
    }
    else
    {
        ; // just fill the string, dont set the type
    }

    token->attribute.strVal = s;

    return SUCCESS;
}

int scanToken(token_t *token)
{
    FILE *fp = stdin; // IFJ22 will be read only from stdin

    machineState_t fsmState = S_START;
    string_t *str = NULL; // string for IDs keywords and string/numeral literals

    int ret = 0;

    int c = 0;

    while (1)
    {
        c = getc(fp);
        switch (fsmState)
        {
        case S_START:
            if (isspace(c))
            {
                ;
            }
            else if (c == '(')
            {
                token->type = TOK_LEFT_PAREN;

                return SUCCESS;
            }
            else if (c == ')')
            {
                token->type = TOK_RIGHT_PAREN;

                return SUCCESS;
            }
            else if (c == ';')
            {
                token->type = TOK_SEMICOLON;

                return SUCCESS;
            }
            else if (c == ':')
            {
                token->type = TOK_COLON;

                return SUCCESS;
            }
            else if (c == ',')
            {
                token->type = TOK_COMMA;

                return SUCCESS;
            }
            else if (c == '{')
            {
                token->type = TOK_LEFT_BRACE;

                return SUCCESS;
            }
            else if (c == '}')
            {
                token->type = TOK_RIGHT_BRACE;

                return SUCCESS;
            }
            else if (c == '!')
            {
                fsmState = S_STRT_NEG_COMP;
            }
            else if (c == '=')
            {
                fsmState = S_ASSIGN;
            }
            else if (c == '<')
            {
                // if there is a char other than '=' or '?' after, it is just the < operator
                int c2 = lookAheadByOneChar(fp);
                if (c2 != '=' && c2 != '?')
                {
                    token->type = TOK_LESS;

                    return SUCCESS;
                }

                fsmState = S_LESSER;
            }
            else if (c == '>')
            {
                if (lookAheadByOneChar(fp) != '=')
                {
                    token->type = TOK_GREATER; // S_GREATER

                    return SUCCESS;
                }
                else
                {
                    token->type = TOK_GREATER_EQUAL; // S_GREATER_EQ

                    c = getc(fp); // move the fp to the next char
                    return SUCCESS;
                }
            }
            else if (c == '+')
            {
                token->type = TOK_PLUS;

                return SUCCESS;
            }
            else if (c == '-')
            {
                token->type = TOK_MINUS;

                return SUCCESS;
            }
            else if (c == '*')
            {
                token->type = TOK_STAR;

                return SUCCESS;
            }
            else if (c == '.')
            {
                token->type = TOK_DOT;

                return SUCCESS;
            }
            else if (c == '"')
            {
                str = stringInit(&ret);
                if (ret == ERR_INTERNAL)
                {
                    return ERR_INTERNAL;
                }
                fsmState = S_STRT_STR;
            }
            else if (c == '_' || isalpha(c))
            {
                str = stringInit(&ret);
                if (ret == ERR_INTERNAL)
                {
                    return ERR_INTERNAL;
                }
                fsmState = S_KEYW_OR_ID;
                ungetc(c, fp);
            }
            else if (c == '?')
            {
                fsmState = S_QSTN_MARK;
            }
            else if (c == '$')
            {
                str = stringInit(&ret);
                if (ret == ERR_INTERNAL)
                {
                    return ERR_INTERNAL;
                }

                fsmState = S_STRT_VAR;
            }
            else if (c == '/')
            {
                fsmState = S_SLASH;
                token->type = TOK_SLASH;
            }
            else if (isdigit(c))
            {
                fsmState = S_INT_LIT;

                str = stringInit(&ret);
                if (ret == ERR_INTERNAL)
                {
                    return ERR_INTERNAL;
                }
                if (charPushBack(str, c) != SUCCESS)
                {
                    return ERR_INTERNAL;
                }
            }
            else if (c == EOF)
            {
                token->type = TOK_EOF;

                return SUCCESS;
            }
            break;
        case S_STRT_NEG_COMP:
            if (c == '=')
            {
                fsmState = S_MID_NEG_COMP;
            }
            else
            {
                return ERR_LEX_ANALYSIS; // S_ERROR
            }
            break;
        case S_MID_NEG_COMP:
            if (c == '=')
            {
                token->type = TOK_NEG_COMPARISON; // S_NEG_COMP

                return SUCCESS;
            }
            else
            {
                return ERR_LEX_ANALYSIS; // S_ERROR
            }
        case S_ASSIGN:
            if (c == '=')
            {
                fsmState = S_STRT_COMP;
            }
            else
            {
                token->type = TOK_ASSIGN; // S_ASSIGN
                ungetc(c, fp); // return the char to the stream (c != '=') 

                return SUCCESS;
            }
            break;
        case S_STRT_COMP:
            if (c == '=')
            {
                token->type = TOK_COMPARISON;

                return SUCCESS; // S_COMPARISON (nonexistent)
            }
            else
            {
                return ERR_LEX_ANALYSIS; // S_ERROR
            }
        case S_LESSER:
            if (c == '=')
            {
                token->type = TOK_LESS_EQUAL;

                return SUCCESS; // S_LESSER_EQ
            }
            else if (c == '?')
            {
                if (!checkForMatch(fp, "php"))
                {
                    token->type = TOK_PROLOGUE; // TOK_PROLOGUE is the whole expression "<?php" TODO should it be taken as syntax error (2) ?

                    return SUCCESS;
                }
                else
                {

                    return ERR_LEX_ANALYSIS; // prologue was not loaded
                }
            }
            break;
        case S_INT_LIT:
            if (isdigit(c))
            {
                if (charPushBack(str, c) != SUCCESS)
                {
                    return ERR_INTERNAL;
                }
            }
            else if (c == '.')
            {
                if (charPushBack(str, c) != SUCCESS)
                {
                    return ERR_INTERNAL;
                }
                fsmState = S_STRT_DEC;
            }
            else if (c == 'E' || c == 'e')
            {
                if (charPushBack(str, c) != SUCCESS)
                {
                    return ERR_INTERNAL;
                }
                fsmState = S_STRT_EXP;
            }
            else
            {
                int res = convertStringToInt(str->str, 10);

                token->type = TOK_INT_LIT;
                token->attribute.intVal = res;

                ungetc(c, fp);

                stringDestroy(str); // free the memory after the token is created
                return SUCCESS;
            }
            break;
        case S_STRT_EXP:
            if (c == '+' || c == '-')
            {
                if (charPushBack(str, c) != SUCCESS)
                {
                    return ERR_INTERNAL;
                }
                fsmState = S_MID_EXP;
                break;
            }
            else if (isdigit(c))
            {
                if (charPushBack(str, c) != SUCCESS)
                {
                    return ERR_INTERNAL;
                }
                fsmState = S_EXP_LIT;
                break;
            }
            else
            {
                stringDestroy(str); // free the memory after encountering lexical error
                return ERR_LEX_ANALYSIS; // S_ERROR, missing number or +-
            }
        case S_MID_EXP:
            if (isdigit(c))
            {
                if (charPushBack(str, c) != SUCCESS)
                {
                    return ERR_INTERNAL;
                }

                fsmState = S_EXP_LIT;
            }
            else
            {
                stringDestroy(str); // free the memory after encountering lexical error
                return ERR_LEX_ANALYSIS; // S_ERROR, missing number
            }
            break;
        case S_EXP_LIT:
            if (isdigit(c))
            {
                if (charPushBack(str, c) != SUCCESS)
                {
                    return ERR_INTERNAL;
                }
            }
            else
            {
                double res = convertStringToDouble(str); 
                token->type = TOK_DEC_LIT;
                token->attribute.decVal = res;

                ungetc(c, fp); // return the char to the stream and end scanning
                stringDestroy(str); // free the memory after the token is created
                return SUCCESS;
            }
            break;
        case S_STRT_DEC:
            if (isdigit(c))
            {
                if (charPushBack(str, c) != SUCCESS)
                {
                    return ERR_INTERNAL;
                }
                fsmState = S_DEC_LIT;
                break;
            }
            else
            {
                stringDestroy(str); // free the memory if encountered a lexical error
                return ERR_LEX_ANALYSIS;
            }
        case S_DEC_LIT:
            if (isdigit(c))
            {
                if (charPushBack(str, c) != SUCCESS)
                {
                    return ERR_INTERNAL;
                }
                break;
            }
            else if (c == 'E' || c == 'e')
            {
                if (charPushBack(str, c) != SUCCESS)
                {
                    return ERR_INTERNAL;
                }
                fsmState = S_STRT_EXP;
                break;
            }
            else
            {
                double res = convertStringToDouble(str);
                token->type = TOK_DEC_LIT;
                token->attribute.decVal = res;

                ungetc(c, fp); // return the char to the stream and end scanning

                stringDestroy(str); // free the memory after the token is created
                return SUCCESS;
            }
        case S_STRT_STR:
            if (c == '"')
            {
                token->type = TOK_STRING_LIT;
                if (str->realLen == 0)
                { // if str is empty, add ""
                    char *tmp = "\"\"";
                    if (strPushBack(str, tmp, 2) != SUCCESS)
                    {
                        return ERR_INTERNAL;
                    }
                }
                token->attribute.strVal = str; // assign the string to the token
                return SUCCESS;
            }
            else if (c == '\\')
            {
                fsmState = S_STRT_ESCP_SQNC;
            }
            else if (c == '$')
            { // dollar sign is not allowed without a backslash before it
                stringDestroy(str); // free the memory if encountered a lexical error
                return ERR_LEX_ANALYSIS;
            }
            else if (c == EOF)
            {
                stringDestroy(str); // free the memory if encountered a lexical error
                return ERR_LEX_ANALYSIS;
            }
            else
            { // else it is part of string literal
                if (charPushBack(str, c) != SUCCESS)
                {
                    return ERR_INTERNAL;
                }
                fsmState = S_STRT_STR;
            }
            break;
        case S_STRT_ESCP_SQNC:;                               // necessary ';', so the declaration of escpStr is valid, and not flagged by gcc
            char escpStr[5] = {'\\', '\0', '\0', '\0', '\0'}; // escape sequence buffer
            escpStr[1] = c;
            if (c == 'x')
            { // hexa escape sequence
                fsmState = S_HEX_SCP_SQNC;
                break;
            }
            else if (c > 47 && c < 57)
            { // [0-8] octal escape seq
                fsmState = S_OCT_SCP_SQNC;
                break;
            }
            else if (c == 't')
            { // single char escape sequences
                if (charPushBack(str, '\t') != SUCCESS)
                {
                    return ERR_INTERNAL;
                }
                fsmState = S_STRT_STR;
                break;
            }
            else if (c == 'n')
            {
                if (charPushBack(str, '\n') != SUCCESS)
                {
                    return ERR_INTERNAL;
                }
                fsmState = S_STRT_STR;
                break;
            }
            else if (c == '"')
            {
                if (charPushBack(str, '"') != SUCCESS)
                {
                    return ERR_INTERNAL;
                }
                fsmState = S_STRT_STR;
                break;
            }
            else if (c == '$')
            {
                if (charPushBack(str, '$') != SUCCESS)
                {
                    return ERR_INTERNAL;
                }
                fsmState = S_STRT_STR;
                break;
            }
            else if (c == '\\')
            {
                if (charPushBack(str, '\\') != SUCCESS)
                {
                    return ERR_INTERNAL;
                }
                fsmState = S_STRT_STR;
                break;
            }
            else if (c == EOF)
            {
                stringDestroy(str); // free the memory if encountered a lexical error
                return ERR_LEX_ANALYSIS;
            }
            if (strPushBack(str, escpStr, 2) != SUCCESS)
            { // if no valid escapes were read, treat it as part of the string
                return ERR_INTERNAL;
            }
            fsmState = S_STRT_STR;
            break;
        case S_HEX_SCP_SQNC:
            escpStr[2] = c;
            if (isdigit(c) || (c > 64 && c < 71) || (c > 96 && c < 103))
            { // [0-9] [a-f] [A-F] first hexa char
                int temp = getc(fp);
                escpStr[3] = temp;
                if (isdigit(temp) || (temp > 64 && temp < 71) || (temp > 96 && temp < 103))
                {                     // second hexa char
                    escpStr[0] = '0'; // making a convertible format
                    temp = convertStringToInt(escpStr, 16);
                    escpStr[0] = '\\'; // in case of pushBack, it'll be in the original form
                    if (temp > 31 && temp < 127)
                    { // if convertible to a printable and allowed char, will do so
                        if (charPushBack(str, temp) != SUCCESS)
                        {
                            return ERR_INTERNAL;
                        }
                        fsmState = S_STRT_STR;
                        break;
                    }
                    else
                    { // else append the escape sequence to the end of the string literal
                        if (strPushBack(str, escpStr, 4) != SUCCESS)
                        {
                            return ERR_INTERNAL;
                        }
                        fsmState = S_STRT_STR;
                        break;
                    }
                }
                else if (temp == EOF)
                { // error caused by EOF
                    stringDestroy(str); // free the memory if encountered a lexical error
                    return ERR_LEX_ANALYSIS;
                }
                else
                { // incorrect hexa escp. seq. -> part of string
                    if (strPushBack(str, escpStr, 4) != SUCCESS)
                    {
                        return ERR_INTERNAL;
                    }
                    fsmState = S_STRT_STR;
                    break;
                }
            }
            else if (c == EOF)
            { // error caused by EOF
                stringDestroy(str); // free the memory if encountered a lexical error
                return ERR_LEX_ANALYSIS;
            }
            else
            { // incorrect hexa escp. seq. -> part of string
                if (strPushBack(str, escpStr, 3) != SUCCESS)
                {
                    return ERR_INTERNAL;
                }
                fsmState = S_STRT_STR;
                break;
            }
        case S_OCT_SCP_SQNC:
            escpStr[2] = c;
            if (c > 47 && c < 57)
            { // checking octal number
                int temp = getc(fp);
                escpStr[3] = temp;
                if (temp > 47 && temp < 57)
                {                     // checking octal number
                    escpStr[0] = '0'; // making a convertible format
                    temp = convertStringToInt(escpStr, 8);
                    escpStr[0] = '\\'; // in case of pushBack, it'll be in the original form
                    if (temp > 31 && temp < 127)
                    { // if convertible to a printable and allowed char, will do so
                        if (charPushBack(str, temp) != SUCCESS)
                        {
                            return ERR_INTERNAL;
                        }
                        fsmState = S_STRT_STR;
                        break;
                    }
                    else
                    { // else append the escape sequence to the end of the string literal
                        if (strPushBack(str, escpStr, 4) != SUCCESS)
                        {
                            return ERR_INTERNAL;
                        }
                        fsmState = S_STRT_STR;
                        break;
                    }
                }
                else if (temp == EOF)
                { // error caused by EOF
                    stringDestroy(str); // free the memory if encountered a lexical error
                    return ERR_LEX_ANALYSIS;
                }
                else
                { // incorrect octal escp. seq. -> part of string
                    if (strPushBack(str, escpStr, 4) != SUCCESS)
                    {
                        return ERR_INTERNAL;
                    }
                    fsmState = S_STRT_STR;
                    break;
                }
            }
            else if (c == EOF)
            { // error caused by EOF
                stringDestroy(str); // free the memory if encountered a lexical error
                return ERR_LEX_ANALYSIS;
            }
            else
            { // incorrect octal escp. seq. -> part of string
                if (strPushBack(str, escpStr, 3) != SUCCESS)
                {
                    return ERR_INTERNAL;
                }
                fsmState = S_STRT_STR;
                break;
            }
        case S_KEYW_OR_ID:
            ungetc(c, fp); // again to manipulate with c correctly
            if (fillStr(str, token, fp, 0) == ERR_INTERNAL)
            {
                return ERR_INTERNAL;
            }

            if (token->type != TOK_DECLARE_STRICT) {
                checkKeyword(token, str); // changes token->type
            }

            return SUCCESS;
        case S_QSTN_MARK:
            if (c == '>')
            {
                token->type = TOK_END_PROLOGUE;

                return SUCCESS;
            }
            else if ((c > 113 && c < 118) || (c > 101 && c < 104) || (c > 109 && c < 112) || (c == 105) || (c == 108) || (c == 97))
            { // hardcoded check for chars allowed in type (pls dont kill me for this if)
                ungetc(c, fp);
                str = stringInit(&ret);
                if (ret == ERR_INTERNAL)
                {
                    return ERR_INTERNAL;
                }

                fsmState = S_TYPE_ID;
                break;
            }
            else
            { // encountered not allowed char in TOK_TYPE_ID
                return ERR_LEX_ANALYSIS; // string not allocated yet, so no need to free it
            }
        case S_TYPE_ID:
            // need to check for 'string', 'int' or 'float'
            ungetc(c, fp);

            if(fillStr(str, token, fp, 2) == ERR_INTERNAL)
            {
                return ERR_INTERNAL;
            }

            if (checkKeyword(token, str) == 0)
            {
                // it doesn't match any keyword, throw err_lex
                stringDestroy(str); // free the memory if encountered a lexical error
                return ERR_LEX_ANALYSIS;
            }
            else if (token->attribute.kwVal == KW_STRING || token->attribute.kwVal == KW_INT || token->attribute.kwVal == KW_FLOAT)
            {
                token->type = TOK_TYPE_ID;

                return SUCCESS; // token->type is set, attribute too
            }
            else
            { // other keywords are not allowed as TYPE but checkKeyword freed string_t, since its a keyword
                return ERR_LEX_ANALYSIS;
            }
            break;
        case S_STRT_VAR:
            if (isalpha(c) || '_')
            {
                charPushBack(str, '$'); // if the scanner gets here, the current token is definetly a var
                ungetc(c, fp);

                if (fillStr(str, token, fp, 1) == ERR_INTERNAL)
                {
                    return ERR_INTERNAL;
                }
                return SUCCESS;
            }
            else
            { // not allowed char in variable name
                stringDestroy(str); // free the memory if encountered a lexical error
                return ERR_LEX_ANALYSIS;
            }
        case S_SLASH:
            if (c == '/')
            {
                fsmState = S_S_COMMENT;
                break;
            }
            else if (c == '*')
            {
                fsmState = S_M_COMMENT;
                break;
            }
            else
            {
                token->type = TOK_SLASH;

                ungetc(c, fp);
                return SUCCESS;
            }
        case S_S_COMMENT:
            if (c == EOF || c == '\n')
            {
                fsmState = S_START;
            }
            break;
        // multiline comments
        case S_M_COMMENT:
            if (c == '*')
            {
                if (lookAheadByOneChar(fp) == '/')
                {
                    fsmState = S_M_COMMENT_FIN;
                }
            }
            else if (c == '\n')
            {
                break;
            }
            else if (c == EOF)
            {
                return ERR_LEX_ANALYSIS; // missing ending of comment
            }

            break;
        case S_M_COMMENT_FIN:
            if (c == '/')
            {
                fsmState = S_START;
            }
            else
            {
                return ERR_LEX_ANALYSIS; // neither nested comment blocks should be there
            }
        }
    }

    return SUCCESS;
}
