/**
 * Project: Translator of language IFJ22
 * @file symtable.cc
 * @author Martin Maršalek - xmarsa15
 * 
 * @brief Implementation of symbol table module for IFJ22
 */

#include "symtable.h"

size_t htab_hash_function(htab_key_t str)
{	
	uint32_t h = 0; // must have 32 bits
	const unsigned char *p;
	for (p = (const unsigned char *)str; *p != '\0'; p++)
		h = 65599 * h + *p;
	return h;
}

htab_t *htab_init(size_t n)
{
	htab_t *htab = malloc(sizeof(htab_t));
	if (htab == NULL)
	{
		return NULL;
	}

	htab->arr_ptr = malloc(sizeof(htab_item_t *) * n);
	if (htab->arr_ptr == NULL)
	{
		free(htab);
		return NULL;
	}

	for (size_t i = 0; i < n; i++)
	{
		htab->arr_ptr[i] = NULL;
	}

	htab->arr_size = n;
	htab->size = 0;

	return htab;
}

size_t htab_size(const htab_t *t)
{
	return t->size;
}

size_t htab_bucket_count(const htab_t *t)
{
	return t->arr_size;
}

void htab_resize(htab_t *t, size_t newn)
{
	if (newn < MINIMUM_SIZE)
	{
		if (t->arr_size == MINIMUM_SIZE)
		{
			return;
		}
		else
		{
			newn = MINIMUM_SIZE;
		}
	}
	// create new htab with the desired size
	htab_t *newT = htab_init(newn);
	if (newT == NULL)
	{
		htab_free(newT);
		return;
	}

	// loop through all elements in the old htab and insert them into the new htab
	for (size_t i = 0; i < t->arr_size; i++)
	{
		htab_item_t *temp = t->arr_ptr[i];
		while (temp != NULL)
		{
			htab_add(newT, temp->pair.key, temp->pair.data);
			temp = temp->next;
		}
	}

	// free contents of the original table, not the table itself
	htab_clear(t);
	free(t->arr_ptr);

	// switch contents of the old table and the new table
	t->arr_ptr = newT->arr_ptr;
	t->size = newT->size;
	t->arr_size = newT->arr_size;

	// free only the temporary table itself, not its contents, as those now belong to the original table
	free(newT);
}

htab_pair_t *htab_find(htab_t *t, htab_key_t key)
{
	uint32_t index = htab_hash_function(key) % t->arr_size;

	htab_item_t *temp = t->arr_ptr[index];
	while (temp != NULL)
	{
		if (strcmp(temp->pair.key, key) == 0)
		{
			return &(temp->pair);
		}

		temp = temp->next;
	}

	return NULL;
}

// data must be malloc'd before calling this function
htab_pair_t *htab_add(htab_t *t, htab_key_t key, token_data_t *data)
{
	uint32_t index = htab_hash_function(key) % t->arr_size;

	if (t->arr_ptr[index] == NULL)
	{
		t->arr_ptr[index] = malloc(sizeof(htab_item_t));
		if (t->arr_ptr[index] == NULL)
		{
			fprintf(stderr, "HTAB ERROR (htab_add): New item at empty index allocation failure");
			return NULL;
		}

		// trim key and dynamically allocate a new one to save space
		char *newKey = malloc(sizeof(char) * (strlen(key) + 1));
		if (newKey == NULL)
		{
			fprintf(stderr, "HTAB ERROR (htab_add): New key string allocation failure");
			return NULL;
		}

		strncpy(newKey, key, strlen(key) + 1);
		newKey[strlen(key)] = '\0'; // add null terminator

		t->arr_ptr[index]->pair.key = newKey;
		t->arr_ptr[index]->pair.data = data;
		t->arr_ptr[index]->next = NULL;
		t->size++;

		if (t->size / t->arr_size > HTAB_AVG_LEN_MAX)
		{
			htab_resize(t, t->arr_size * 2);
			// since the new tab has newly allocated items, we need to find where the item was reallocated
			return htab_find(t, key);
		}

		return &t->arr_ptr[index]->pair;
	}

	// search the table using the index and the key
	htab_item_t **temp = &t->arr_ptr[index]; // double pointer necessary in order to change arr_ptr[index] itself
	while (*temp != NULL)
	{
		if (strcmp((*temp)->pair.key, key) == 0)
		{
			return NULL; // item with that key already exists
		}

		temp = &(*temp)->next;
	}

	*temp = malloc(sizeof(htab_item_t));
	if (*temp == NULL)
	{
		return NULL;
	}

	// trim key and dynamically allocate a new one to save space
	char *newKey = malloc(sizeof(char) * (strlen(key) + 1));
	if (newKey == NULL)
	{
		fprintf(stderr, "HTAB ERROR (htab_add): New key string allocation failure");
		return NULL;
	}

	strncpy(newKey, key, strlen(key) + 1);
	(*temp)->pair.key = newKey;
	(*temp)->pair.data = data;
	(*temp)->next = NULL;
	t->size++;

	if (t->size / t->arr_size > HTAB_AVG_LEN_MAX)
	{
		htab_resize(t, t->arr_size * 2);
		// since the new tab has newly allocated items, we need to find where the item was reallocated
		return htab_find(t, key);
	}

	return &(*temp)->pair;
}

bool htab_erase(htab_t *t, htab_key_t key)
{
	uint32_t index = htab_hash_function(key) % t->arr_size;

	// double pointer necessary in order to change arr_ptr[index] itself
	htab_item_t **temp = &t->arr_ptr[index];
	while (*temp != NULL)
	{
		if (strcmp((*temp)->pair.key, key) == 0)
		{
			// found
			htab_item_t *newNext = (*temp)->next;
			free((char *)(*temp)->pair.key);
			free((*temp)->pair.data);
			free(*temp);
			t->size--;

			*temp = newNext;

			if (t->size / t->arr_size < HTAB_AVG_LEN_MIN)
			{
				htab_resize(t, t->arr_size / 2);
			}

			return true;
		}

		temp = &(*temp)->next;
	}

	return false;
}

void htab_for_each(const htab_t *t, void (*f)(htab_pair_t *data))
{
	for (size_t i = 0; i < t->arr_size; i++)
	{
		htab_item_t *temp = t->arr_ptr[i];
		while (temp != NULL)
		{
			f(&temp->pair);
			temp = temp->next;
		}
	}
}

void htab_clear(htab_t *t)
{
	for (size_t i = 0; i < t->arr_size; i++)
	{
		// basically an adjusted htab_erase for efficiency
		htab_item_t **temp = &t->arr_ptr[i];
		while (*temp != NULL)
		{
			// save pointer to next, so we don't lose it after calling free
			htab_item_t *newNext = (*temp)->next;
			free((char *)(*temp)->pair.key);
			free((*temp)->pair.data);
			free(*temp);

			*temp = newNext;
		}
	}

	// resize htab to minimum size
	t->size = 0;
	free(t->arr_ptr);
	t->arr_ptr = malloc(sizeof(htab_item_t *) * HTAB_MINIMUM_SIZE);
	t->arr_size = HTAB_MINIMUM_SIZE;
}

void htab_free(htab_t *t)
{
	htab_clear(t);
	free(t->arr_ptr);
	free(t);

	t = NULL;
}