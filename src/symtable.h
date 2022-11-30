// hash_tab.h -- rozhraní knihovny htab (řešení IJC-DU2)
// Licence: žádná (Public domain)

#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <string.h>	 // size_t
#include <stdbool.h> // bool
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "token.h"

#define HTAB_MINIMUM_SIZE 23
#define HTAB_AVG_LEN_MIN 32
#define HTAB_AVG_LEN_MAX 128
#define MINIMUM_SIZE 23

/** @brief Types of token data */
typedef enum
{
	DATA_UNDEFINED, DATA_INT, DATA_FLOAT, DATA_STRING, DATA_NULL
} data_type_t;

/** @brief Types of token data */
typedef struct
{
	data_type_t type;
	char* name;
} parameter_t;

typedef struct
{
	parameter_t *vector;
	size_t length; // maximum amount of elements (space allocated)
	size_t size; // current amount of real elements
} param_list_t;

/** @brief Function structure */
typedef struct
{
	param_list_t* param_list;
	data_type_t return_type;
	bool defined;
} sym_func_t;

typedef const char *htab_key_t;

/** @brief Variable structure */
typedef struct
{
	data_type_t type;
} sym_var_t;

typedef const char *htab_key_t;

// /** @brief Constant structure */
// typedef struct
// {
// 	data_type_t type;
// 	union
// 	{
// 		int value_int;
// 		float value_float;
// 		const char* value_string;
// 		bool value_null; // TODO: how to represent null?
// 	} value;
// } sym_const_t;

typedef const char *htab_key_t;

/** @brief Types of token */
typedef enum
{
	TOKTYPE_FUNCTION, TOKTYPE_VARIABLE, TOKTYPE_CONSTANT
} token_type_t;

typedef const char *htab_key_t;

/** @brief Structure of token data */
typedef struct token_data
{
	const char *ID;
	token_type_t type;
	union
	{
		sym_func_t function;
		sym_var_t variable;
	} data;
} token_data_t;

/** @brief Structure of a hash table item's content */
typedef struct htab_pair
{
	htab_key_t key;
	token_data_t *data;
} htab_pair_t;

/** @brief Structure of a hash table item */
typedef struct htab_item
{
	htab_pair_t pair;
	struct htab_item* next;
} htab_item_t;

/** @brief Structure of hash table */
typedef struct htab
{
	size_t size;
	size_t arr_size;
	htab_item_t **arr_ptr;
} htab_t;

/** 
 * @brief Hash key value
 * @param str key string to hash
 * 
 * @return result of the hash function
 */
size_t htab_hash_function(htab_key_t str);

/** 
 * @brief Initialize hash table
 * @param n number of buckets
 * 
 * @return pointer to initialized hash table, or NULL if an error occured
 */
htab_t *htab_init(size_t n);

/** 
 * @brief Returns the number of hash table items
 * @param t pointer to a hash table
 * 
 * @return SUCCESS, otherwise ERR_INTERNAL
 */
size_t htab_size(const htab_t *t);

/** 
 * @brief Allocate memory for token
 * @param t pointer to a hash table
 * 
 * @return SUCCESS, otherwise ERR_INTERNAL
 */
size_t htab_bucket_count(const htab_t *t);

/** 
 * @brief Resize the hash table
 * @param t pointer to a hash table
 * @param newn number of buckets
 */
void htab_resize(htab_t *t, size_t newn);

/** 
 * @brief Find an item in the hash table
 * @param t pointer to a hash table
 * @param key key value of the desired item
 * 
 * @return pointer to hash table item, or NULL if not found
 */
htab_pair_t *htab_find(htab_t *t, htab_key_t key);

/** 
 * @brief Add an item to the hash table
 * @param t pointer to a hash table
 * @param key key of the new item
 * @param data data of the new item
 * @param type data type of the new item
 * 
 * @return pointer to the new hash table item, or NULL if an item with the key already exists
 */
htab_pair_t *htab_add(htab_t *t, htab_key_t key, token_data_t *data);

/** 
 * @brief Erase an item in the hash table
 * @param t pointer to a hash table
 * @param key key value of the desired item
 * 
 * @return pointer to hash table item, or NULL if not found
 */
bool htab_erase(htab_t *t, htab_key_t key);

void htab_for_each(const htab_t *t, void (*f)(htab_pair_t *data));

/** 
 * @brief Clear hash table
 * @param t pointer to a hash table
 */
void htab_clear(htab_t *t);

/** 
 * @brief Free hash table
 * @param t pointer to a hash table
 */
void htab_free(htab_t *t);

#endif /* SYMTABLE_H */