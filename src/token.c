#include "token.h"

token_data_t *createTokenDataFunction(char *ID, data_type_t *param_list, int param_count, data_type_t return_type, bool defined)
{
	token_data_t *data = malloc(sizeof(token_data_t));
	if (data == NULL)
	{
		fprintf(stderr, "[ERROR] Token data allocation error.\n");
		exit(ERR_SEM_OTHER);
	}

	data->type = TOKTYPE_FUNCTION;
	data->ID = ID;
	data->data.function.param_list = param_list;
	data->data.function.defined = defined;
	data->data.function.param_count = param_count;
	data->data.function.return_type = return_type;
}

token_data_t *createTokenDataVariable(char *ID, data_type_t type)
{
	token_data_t *data = malloc(sizeof(token_data_t));
	if (data == NULL)
	{
		fprintf(stderr, "[ERROR] Token data allocation error.\n");
		exit(ERR_SEM_OTHER);
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
		exit(ERR_SEM_OTHER);
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
		exit(ERR_SEM_OTHER);
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
		exit(ERR_SEM_OTHER);
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
		exit(ERR_SEM_OTHER);
	}

	data->type = TOKTYPE_CONSTANT;
	data->ID = ID;
	data->data.constant.type = DATA_NULL;
	data->data.constant.value.value_null = true;
}