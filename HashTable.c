#include <stdlib.h>
#include <string.h>
#include <stdio.h>

extern char *strdup(const char *); // strdup not declared in C99

#include "Dict.h"

#define REALLOC_THRESHOLD 0.7f

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
    Node *node = dictGet(d, key);

    if (node)
        node->data = data;
    else
    {
        // Si le tableau a encore de la place (en-dessous du seuil) on ajoute le noeud
        if ((float)d->nbKeys / d->size < REALLOC_THRESHOLD)
        {
            node = malloc(sizeof(Node));
            if (!node)
            {
                printf("Allocation error in dictInsert!\n");
                return;
            }
            size_t i = hash(key) % d->size;
            node->key = strdup(key);
            node->data = data;
            node->next = d->array[i];
            d->array[i] = node;
            d->nbKeys++;
        }
        else // Sinon on agrandit le tableau
        {
            // Déclaration des variables nécessaires
            Node *next_node = NULL;
            // Déclaration du nouveau tableau
            size_t new_size = d->size * 2;
            Node **new_array = calloc(d->size * 2, sizeof(Node));

            for (size_t j = 0; j < d->size; j++)
            {
                next_node = d->array[j];
                while (next_node != NULL)
                {
                    node = next_node ; // On réutilise l'ancien noeud
                    next_node = node->next; // On prépare la prochaine itération
                    //On insère le noeud dans le nouveau tableau (on garde sa clé et sa donnée)
                    size_t i = hash(node->key) % new_size;
                    node->next = new_array[i];
                    new_array[i] = node;
                }
            }
            // On a plus besoin de l'ancien tableau
            free(d->array);
            // Nouveau tableau et nouvelle taille
            d->array = new_array;
            d->size = new_size;
        }
    }
}
