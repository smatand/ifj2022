#define CALL_RULE(ruleFunction)           \
	do                                    \
	{                                     \
		int error = ruleFunction(parser); \
		if (error != 0)                   \
			return error;                 \
	} while (0)

#define CURRENT_TOKEN_TYPE(tokenType)                                \
	do                                                               \
	{                                                                \
		int check = checkTokenType(parser->currentToken, tokenType); \
		if (check != 0)                                              \
			return check;                                            \
	} while (0)

#define CURRENT_TOKEN_KWORD(keyword)                                  \
	do                                                                \
	{                                                                 \
		int check = checkTokenKeyword(parser->currentToken, keyword); \
		if (check != 0)                                               \
			return check;                                             \
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