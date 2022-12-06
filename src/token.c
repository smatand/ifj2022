#include <stdlib.h>

#include "token.h"

char *createTokenKey(string_t* ID)
{
	char * newID = malloc((ID->realLen + 1)); // copy the name of the function, and leave some space
	if (newID == NULL) {
		return NULL;
	}
	
	memcpy(newID, ID->str, ID->realLen);
	newID[ID->realLen] = '\0';

	return newID;
}

token_data_t *createTokenDataFunction()
{
	struct token_data *data = malloc(sizeof(struct token_data));
	if (data == NULL)
	{
		exit(ERR_INTERNAL);
	}

	data->type = TOKTYPE_FUNCTION;

	data->param_IDs = malloc(sizeof(char*) * MAX_PARAM_COUNT); // TODO: memory leak, uninitialized pointers
	if (data->param_IDs == NULL)
	{
		free(data);
		exit(ERR_INTERNAL);
	}

	return data;
}

struct token_data *createTokenDataVariable()
{
	struct token_data *data = malloc(sizeof(struct token_data));
	if (data == NULL)
	{
		fprintf(stderr, "[ERROR] Token data allocation error.\n");
		exit(ERR_INTERNAL);
	}

	data->type = TOKTYPE_VARIABLE;
	data->param_IDs = NULL;
	return data;
}
