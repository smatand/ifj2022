/**
 * @brief Calls function and returns error code if it fails
 * @param ruleFunction Function to call
 * 
 * @return caught error code, otherwise nothing
*/
#define CALL_RULE(ruleFunction)           \
	do                                    \
	{                                     \
		int ret = ruleFunction(parser);   \
		if (ret != SUCCESS)               \
			return ret;                	  \
	} while (0)

/**
 * @brief Calls function and gets next token if it succeeds
 * @param ruleFunction Function to call
 * 
 * @return caught error code, otherwise nothing
*/
#define CALL_RULE_GETNEXT(ruleFunction)           \
	do                                    \
	{                                     \
		int ret = ruleFunction(parser);   \
		if (ret != SUCCESS)               \
			return ret;                	  \
		getNextToken(parser);             \
	} while (0)

/**
 * @brief Checks type of current token
 * @param tokenType Type of token to check against
 * 
 * @return 0 if matching, otherwise non-zero value
*/
#define CURRENT_TOKEN_TYPE(tokenType)                                \
	do                                                               \
	{                                                                \
		int check = checkTokenType(parser->currentToken, tokenType); \
		if (check != 0)                                              \
			return ERR_SYN_ANALYSIS;                                 \
	} while (0)

/**
 * @brief Checks type of current token and gets the next
 * @param tokenType Type of token to check against
 * 
 * @return caught error code, otherwise nothing
*/
#define CURRENT_TOKEN_TYPE_GETNEXT(tokenType)                        \
	do                                                               \
	{                                                                \
		int check = checkTokenType(parser->currentToken, tokenType); \
		if (check != 0)                                              \
			return ERR_SYN_ANALYSIS;                                 \
																	 \
		int retValue = getNextToken(parser);						 \
		if (retValue) 												 \
			return retValue;										 \
	} while (0)

/**
 * @brief Checks the keyword type of current token 
 * @param keyword Type of keyword to check against
 * 
 * @return caught error code, otherwise nothing
*/
#define CURRENT_TOKEN_KWORD(keyword)                                  \
	do                                                                \
	{                                                                 \
		int check = checkTokenKeyword(parser->currentToken, keyword); \
		if (check != 0)                                               \
			return ERR_SYN_ANALYSIS;                                  \
	} while (0)

/**
 * @brief Checks the keyword type of current token and gets the next
 * @param keyword Type of keyword to check against
 * 
 * @return caught error code, otherwise nothing
*/
#define CURRENT_TOKEN_KWORD_GETNEXT(keyword)                          \
	do                                                                \
	{                                                                 \
		int check = checkTokenKeyword(parser->currentToken, keyword); \
		if (check != 0)                                               \
			return ERR_SYN_ANALYSIS;                                  \
		getNextToken(parser);                                         \
	} while (0)

/**
 * @brief Checks the type of next token
 * @param tokenType Type of token to check against
 * 
 * @return caught error code, otherwise nothing
*/
#define NEXT_TOKEN_TYPE(tokenType)                                \
	do                                                            \
	{                                                             \
		int check = checkTokenType(parser->nextToken, tokenType); \
		if (check != 0)                                           \
			return ERR_SYN_ANALYSIS;                              \
	} while (0)

/**
 * @brief Checks the keyword type of next token
 * @param tokenType Type of token to check against
 * 
 * @return caught error code, otherwise nothing
*/
#define NEXT_TOKEN_KWORD(keyword)                                  \
	do                                                             \
	{                                                              \
		int check = checkTokenKeyword(parser->nextToken, keyword); \
		if (check != 0)                                            \
			return ERR_SYN_ANALYSIS;                               \
	} while (0)