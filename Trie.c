#include <stdlib.h>
#include <string.h>

#include "Dict.h"

typedef struct Node_t
{
    struct Node_t *children;
    struct Node_t *parent;
    struct Node_t *nextchild;
    char label;
    char *data;
} Node;

struct Dict_t
{
    Node *root;
    Node **array;
    size_t size;
    size_t nbkeys;
};

static Node *dictGet(Dict *d, const char *key);
static void init_node(Node *node, Node *children, Node *parent, Node *nextchild, char key, char *data);
static void append_node(Dict *d, Node *node);
static const char *dictSearchLongest_rec(Node *children, const char *letters, const char *longest);

static Node *dictGet(Dict *d, const char *key)
{
    size_t index = 0;
    Node *next_node = d->root;
    while (next_node && key[index] != '\0') // Max strlen(key)
    {
        next_node = next_node->children; // On regarde les prochaines lettres stockées
        if (!next_node)
            break;
        while (next_node) // Max 26 tours (nbre de char possibles)
        {
            if (next_node->label == key[index])
                break;
            else
                next_node = next_node->nextchild;
        }
        index++; // On regarde la prochaine lettre de "key"
    }
    return next_node;
}

static void init_node(Node *node, Node *children, Node *parent, Node *nextchild, char label, char *data)
{
    node->children = children;
    node->parent = parent;
    node->nextchild = nextchild;
    node->label = label;
    node->data = data;
}

static void append_node(Dict *d, Node *node)
{
    if (d->nbkeys + 1 == d->size)
    {
        d->size = 2 * d->size; // On multiplie pour avoir une complexité linéaire
        d->array = realloc(d->array, d->size * sizeof(Node *));
    }
    d->array[d->nbkeys] = node;
    d->nbkeys++;
}

Dict *dictCreateEmpty()
{
    Dict *dict = malloc(sizeof(Dict));
    Node *root = malloc(sizeof(Node));
    init_node(root, NULL, NULL, NULL, '\0', NULL);

    dict->root = root;
    dict->size = 10;
    dict->array = calloc(dict->size, sizeof(Node *));
    dict->nbkeys = 1;
    dict->array[0] = root;
    return dict;
}

size_t dictNbKeys(Dict *d)
{
    return d->nbkeys;
}

void dictFree(Dict *d)
{
    for (size_t i = d->nbkeys; i != 0; i--)
        free(d->array[i - 1]);
    free(d->array);
    free(d);
}

int dictContains(Dict *d, const char *key)
{
    return dictSearch(d, key) != NULL;
}

void *dictSearch(Dict *d, const char *key)
{
    Node *node = dictGet(d, key);
    return node ? node->data : NULL; // NULL si aucun mot n'a été trouvé
}

static const char *dictSearchLongest_rec(Node *children, const char *letters, const char *longest)
{
    if (strcmp(letters, "") == 0 || children == NULL)
        return longest;
    Node *child = children;
    char *subletters = NULL;
    const char *candidate = NULL;
    while (child)
    {
        subletters = strchr(letters, child->label);
        if (subletters)
        {
            if (child->data && strlen(child->data) > strlen(longest))
                longest = child->data;
            candidate = dictSearchLongest_rec(child->children, subletters + 1, longest);
            if (strlen(candidate) > strlen(longest))
                longest = candidate;
        }
        child = child->nextchild;
    }
    return longest;
}

void *dictSearchLongest(Dict *d, const char *letters)
{
    const char *longest = dictSearchLongest_rec(d->root->children, letters, "");
    return strcmp(longest, "") == 0 ? NULL : longest;
}

void dictInsert(Dict *d, const char *key, void *data)
{
    Node *node = dictGet(d, key);
    if (!node)
    {
        size_t index = 0;
        Node *next_node = d->root, *prev_child = NULL, *parent_node = NULL, *new_node = NULL;
        while (key[index] != '\0') // Max strlen(key)
        {
            parent_node = next_node;
            next_node = parent_node->children; // On regarde les prochaines lettres stockées
            while (next_node)                  // Max 26 tours (nbre de char possibles)
            {
                if (next_node->label == key[index])
                    break;
                prev_child = next_node;
                next_node = next_node->nextchild;
            }
            if (!next_node)
            {
                new_node = malloc(sizeof(Node));
                init_node(new_node, NULL, parent_node, NULL, key[index], NULL);
                if (key[index + 1] == '\0') // Est-on à la dernière lettre de "key"?
                    new_node->data = data;
                if (parent_node->children) // Pas le premier fils
                    prev_child->nextchild = new_node;
                else // Premier fils
                    parent_node->children = new_node;
                append_node(d, new_node);
                next_node = new_node; // On prépare la prochaine itération
            }
            index++; // Prochaine lettre de "key"
        }
    }
}
