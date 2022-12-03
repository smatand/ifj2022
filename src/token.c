#include <stdlib.h>

#include "token.h"

char *createTokenKey(string_t* ID)
{
	char* newID = malloc(sizeof(char) * (ID->realLen + TOKEN_KEY_SPECIFIER_SIZE + 1)); // copy the name of the function, and leave some space
	strncpy(&newID[TOKEN_KEY_SPECIFIER_SIZE], ID->str, ID->realLen + 1); // +1 to include the null terminator

	return newID;
}

token_data_t *createTokenDataFunction(char *ID)
{
	struct token_data *data = malloc(sizeof(struct token_data));
	if (data == NULL)
	{
		fprintf(stderr, "[ERROR] Token data allocation error.\n");
		exit(ERR_INTERNAL);
	}

	data->type = TOKTYPE_FUNCTION;
	data->ID = ID;

	return data;
}

struct token_data *createTokenDataVariable(char *ID)
{
	struct token_data *data = malloc(sizeof(struct token_data));
	if (data == NULL)
	{
		fprintf(stderr, "[ERROR] Token data allocation error.\n");
		exit(ERR_INTERNAL);
	}

	data->type = TOKTYPE_VARIABLE;
	data->ID = ID;

	return data;
}
