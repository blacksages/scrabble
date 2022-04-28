#include <stdlib.h>
#include <string.h>

extern char *strdup(const char *); // strdup not declared in C99

#include "Dict.h"

typedef struct Node_t
{
    char *key;
    void *data;
    struct Node_t *next;
} Node;

struct Dict_t
{
    size_t size;
    size_t nbKeys;
    Node **array;
};

static size_t hash(const char *key)
{
    size_t length = strlen(key);
    size_t sum = 0, base = 1, mod = 1000000007;

    for (size_t i = 0; i < length; i++)
    {
        sum = (sum + base * key[i]) % mod;
        base = (base * 256) % mod;
    }

    return sum;
}

Dict *dictCreateEmpty()
{
    Dict *d = malloc(sizeof(Dict));
    if (!d)
        return NULL;

    d->size = 1000;
    d->nbKeys = 0;
    d->array = calloc(d->size, sizeof(Node));
    if (!d->array)
    {
        free(d);
        return NULL;
    }

    return d;
}

size_t dictNbKeys(Dict *d)
{
    return d->nbKeys;
}

void dictFree(Dict *d)
{
    Node *a;
    Node *b;

    for (size_t i = 0; i < d->size; i++)
    {
        a = d->array[i];

        while (a)
        {
            b = a->next;
            free(a->key);
            free(a);
            a = b;
        }
    }

    free(d->array);
    free(d);
}

static Node *dictGet(Dict *d, const char *key)
{
    Node *n = d->array[hash(key) % d->size];

    while (n && strcmp(n->key, key) != 0)
        n = n->next;

    return n;
}

int dictContains(Dict *d, const char *key)
{
    return dictGet(d, key) != NULL;
}

void *dictSearch(Dict *d, const char *key)
{
    Node *n = dictGet(d, key);

    if (n)
        return n->data;
    else
        return NULL;
}

void dictInsert(Dict *d, const char *key, void *data)
{
    Node *n = dictGet(d, key);

    if (n)
        n->data = data;
    else
    {
        size_t i = hash(key) % d->size;

        n = malloc(sizeof(Node));
        n->key = strdup(key);
        n->data = data;
        n->next = d->array[i];
        d->array[i] = n;
        d->nbKeys++;

				//If the table is above the defined max filling threshold
				float tmp = (((float)d->nbKeys/(float)d->size)*100);
				if(tmp > (float)REALLOC_THRESHOLD){
					//Declaring useful variables
					size_t new_size = d->size * 2;
					Node **new_array = calloc(d->size * 2, sizeof(Node));
					size_t j = 0;
					size_t new_index;
					char *tmp_key;
					Node *tmp_next;

					//For each node, we redefine its position via the hash function and
					//the new size, and put it at this new position in a new array (given current node is not null);
					for(j = 0; j < d->size; j++){
						n = d->array[j];
						if(n != NULL){
							while(n->next != NULL){
								//We recover the key and the next node
								tmp_key = n->key;
								tmp_next = n->next;

								//We compute new position and create a bucket there
								new_index = hash(tmp_key) % new_size;
								n->next = new_array[new_index];
								new_array[new_index] = n;

								//Iterate loop
								n = tmp_next;
							}
						}
					}
					//Putting the new array in the structure, and updating the size
					d->array = new_array;
					d->size = new_size;

					//We don't need the old array so we free it
				}
    }
}
