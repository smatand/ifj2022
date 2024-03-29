/**
 * Project: Translator of language IFJ22
 * @file symtable.h
 * @author Martin Maršalek - xmarsa15
 * 
 * @brief Header file of symbol table module for IFJ22
 * 
 * Used citations of code:
 * 1.	hash_tab.h -- rozhraní knihovny htab (řešení IJC-DU2)
 * 		license: none (Public domain)
 */

#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <string.h>	 // size_t
#include <stdbool.h> // bool
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define HTAB_MINIMUM_SIZE 23
#define HTAB_AVG_LEN_MIN 32
#define HTAB_AVG_LEN_MAX 128
#define MINIMUM_SIZE 23

#define INITIAL_BUCKET_COUNT 113


typedef const char *htab_key_t;

/** @brief Types of token */
typedef enum
{
	TOKTYPE_FUNCTION, TOKTYPE_VARIABLE
} token_type_t;

/** @brief Structure of token data */
typedef struct token_data
{
	token_type_t type;
	char **param_IDs; // array of strings
	int param_count;
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
 * @brief Returns the number of items in hash table
 * @param t pointer to a hash table
 * 
 * @return number of items
 */
size_t htab_size(const htab_t *t);

/** 
 * @brief Returns the number of buckets in hash table
 * @param t pointer to a hash table
 * 
 * @return the number of buckets
 */
size_t htab_bucket_count(const htab_t *t);

/** 
 * @brief Resize the hash table
 * @param t pointer to a hash table
 * @param newn new number of buckets
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
 * 
 * @return pointer to the new hash table item, or NULL if an item with the key already exists
 */
htab_pair_t *htab_add(htab_t *t, htab_key_t key, token_data_t *data);

/** 
 * @brief Erase an item in the hash table
 * @param t pointer to a hash table
 * @param key key value of the desired item
 * 
 * @return true if an item is deleted, otherwise false
 */
bool htab_erase(htab_t *t, htab_key_t key);


/** 
 * @brief Call function on all items in hash table
 * @param t pointer to a hash table
 * @param f pointer to function to be applied
 * 
 * @return true if an item is deleted, otherwise false
 */
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
