#define CALL_RULE(ruleFunction)           \
	do                                    \
	{                                     \
		int ret = ruleFunction(parser);   \
		if (ret != SUCCESS)               \
			return ret;                	  \
	} while (0)

#define CURRENT_TOKEN_TYPE(tokenType)                                \
	do                                                               \
	{                                                                \
		int check = checkTokenType(parser->currentToken, tokenType); \
		if (check != 0)                                              \
			return check;                                            \
	} while (0)

#define CURRENT_TOKEN_TYPE_GETNEXT(tokenType)                        \
	do                                                               \
	{                                                                \
		int check = checkTokenType(parser->currentToken, tokenType); \
		if (check != 0)                                              \
			return check;                                            \
		getNextToken(parser);                                        \
	} while (0)

#define CURRENT_TOKEN_KWORD(keyword)                                  \
	do                                                                \
	{                                                                 \
		int check = checkTokenKeyword(parser->currentToken, keyword); \
		if (check != 0)                                               \
			return check;                                             \
	} while (0)

#define CURRENT_TOKEN_KWORD_GETNEXT(keyword)                          \
	do                                                                \
	{                                                                 \
		int check = checkTokenKeyword(parser->currentToken, keyword); \
		if (check != 0)                                               \
			return check;                                             \
		getNextToken(parser);                                         \
	} while (0)

#define NEXT_TOKEN_TYPE(tokenType)                                \
	do                                                            \
	{                                                             \
		int check = checkTokenType(parser->nextToken, tokenType); \
		if (check != 0)                                           \
			return check;                                         \
	} while (0)

#define NEXT_TOKEN_KWORD(keyword)                                  \
	do                                                             \
	{                                                              \
		int check = checkTokenKeyword(parser->nextToken, keyword); \
		if (check != 0)                                            \
			return check;                                          \
	} while (0)