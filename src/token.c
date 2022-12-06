#include <stdlib.h>

#include "token.h"
#include <stdlib.h>

char *createTokenKey(string_t *ID)
{
	char *newID = malloc((ID->realLen + 1)); // copy the name of the function, and leave some space
	if (newID == NULL)
	{
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
	data->param_count = 0;

	data->param_IDs = calloc(sizeof(char *) * MAX_PARAM_COUNT, 1); // TODO: memory leak
	if (data->param_IDs == NULL)
	{
		free(data);
		exit(ERR_INTERNAL);
	}
	//for (int i = 0; i < MAX_PARAM_COUNT; i++)
	//{
	//	data->param_IDs = NULL; // set all the pointers to NULL
	//}

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
	data->param_count = -1;
	return data;
}

void functionAddParam(token_data_t *funcData, char *paramID)
{
	if (funcData->param_count < MAX_PARAM_COUNT)
	{
		int length = strlen(paramID);
		char *newID = malloc(sizeof(char) * (length + 1)); // copy the name of the function, and leave some space
		if (newID == NULL)
		{
			fprintf(stderr, "[ERROR] Internal error.\n");
			exit(ERR_INTERNAL);
		}

		memcpy(newID, paramID, length);
		newID[length + 1] = '\0';

		funcData->param_IDs[funcData->param_count] = newID;
		funcData->param_count++;
	}
}
