#include "token.h"

char *createTokenKey(string_t* ID, token_type_t type)
{
	// token key takes the form x#id, where x can be f (function), v (variable), c (constant)

	char* newID = malloc(sizeof(char) * (ID->realLen + TOKEN_KEY_SPECIFIER_SIZE + 1)); // copy the name of the function, and leave some space
	strncpy(&newID[TOKEN_KEY_SPECIFIER_SIZE], ID->str, ID->realLen + 1); // +1 to include the null terminator

	switch (type)
	{
		case TOKTYPE_FUNCTION:
			newID[0] = 'f';
			break;
		case TOKTYPE_VARIABLE:
			newID[0] = 'v';
			break;
		case TOKTYPE_CONSTANT:
			newID[0] = 'c';
			break;
		default:
			fprintf(stderr, "[ERROR] Error in createTokenKey switch.\n");
			exit(ERR_INTERNAL);
	}

	newID[1] = '#';

	return newID;
}

param_list_t *createParamList()
{
	param_list_t *param_list = malloc(sizeof(param_list_t));
	if (param_list == NULL)
	{
		fprintf(stderr, "[ERROR] Parameter list allocation error.\n");
		exit(ERR_INTERNAL);
	}

	param_list->vector = malloc(sizeof(parameter_t) * BASE_PARAM_LIST_SIZE);
	if (param_list->vector == NULL)
	{
		fprintf(stderr, "[ERROR] Parameter list vector allocation error.\n");
		exit(ERR_INTERNAL);
	}
	
	param_list->length = 0;
	param_list->size = BASE_PARAM_LIST_SIZE;
}

void addToParamList(param_list_t *param_list, data_type_t data_type, string_t* name)
{
	if (param_list->length >= param_list->size)
	{
		param_list->vector = realloc(param_list->vector, param_list->length * 2);
		if (param_list->vector == NULL)
		{
			fprintf(stderr, "[ERROR] Parameter list vector reallocation error.\n");
			exit(ERR_INTERNAL);
		}
		param_list->length *= 2;
	}

	char* newID = malloc(sizeof(char) * (name->realLen + 1)); // copy the name of the function, and leave some space
	strncpy(newID, name->str, name->realLen + 1); // +1 to include the null terminator

	param_list->vector[param_list->size].type = data_type;
	param_list->vector[param_list->size].name = newID;
	param_list->size++;
}

void destroyParamList(param_list_t *param_list)
{
	for (int i = 0; i < param_list->size; i++)
	{
		free(param_list->vector[i].name);
	}

	free(param_list->vector);
	free(param_list);
}

token_data_t *createTokenDataFunction(char *ID, param_list_t *param_list, data_type_t return_type, bool defined)
{
	token_data_t *data = malloc(sizeof(token_data_t));
	if (data == NULL)
	{
		fprintf(stderr, "[ERROR] Token data allocation error.\n");
		exit(ERR_INTERNAL);
	}

	data->type = TOKTYPE_FUNCTION;
	data->ID = ID;
	data->data.function.param_list = param_list;
	data->data.function.defined = defined;
	data->data.function.return_type = return_type;
}

token_data_t *createTokenDataVariable(char *ID, data_type_t type)
{
	token_data_t *data = malloc(sizeof(token_data_t));
	if (data == NULL)
	{
		fprintf(stderr, "[ERROR] Token data allocation error.\n");
		exit(ERR_INTERNAL);
	}

	data->type = TOKTYPE_VARIABLE;
	data->ID = ID;
	data->data.variable.type = type;
}

token_data_t *createTokenDataConstantInt(char *ID, int value)
{
	token_data_t *data = malloc(sizeof(token_data_t));
	if (data == NULL)
	{
		fprintf(stderr, "[ERROR] Token data allocation error.\n");
		exit(ERR_INTERNAL);
	}

	data->type = TOKTYPE_CONSTANT;
	data->ID = ID;
	data->data.constant.type = DATA_INT;
	data->data.constant.value.value_int = value;
}

token_data_t *createTokenDataConstantFloat(char *ID, float value)
{
	token_data_t *data = malloc(sizeof(token_data_t));
	if (data == NULL)
	{
		fprintf(stderr, "[ERROR] Token data allocation error.\n");
		exit(ERR_INTERNAL);
	}

	data->type = TOKTYPE_CONSTANT;
	data->ID = ID;
	data->data.constant.type = DATA_FLOAT;
	data->data.constant.value.value_float = value;
}

token_data_t *createTokenDataConstantString(char *ID, char *value)
{
	token_data_t *data = malloc(sizeof(token_data_t));
	if (data == NULL)
	{
		fprintf(stderr, "[ERROR] Token data allocation error.\n");
		exit(ERR_INTERNAL);
	}

	data->type = TOKTYPE_CONSTANT;
	data->ID = ID;
	data->data.constant.type = DATA_STRING;
	data->data.constant.value.value_string = value;
}

token_data_t *createTokenDataConstantNull(char *ID)
{
	token_data_t *data = malloc(sizeof(token_data_t));
	if (data == NULL)
	{
		fprintf(stderr, "[ERROR] Token data allocation error.\n");
		exit(ERR_INTERNAL);
	}

	data->type = TOKTYPE_CONSTANT;
	data->ID = ID;
	data->data.constant.type = DATA_NULL;
	data->data.constant.value.value_null = true;
}