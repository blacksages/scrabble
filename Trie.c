#include <stdlib.h>
#include <string.h>

#include "Dict.h"

/**
 * @brief Noeud d'un arbre n-aire.
 *        Issu du cours de SDA, slide 223.
 *
 */
typedef struct TrieNode_t
{
    struct TrieNode_t *children;
    struct TrieNode_t *parent;
    struct TrieNode_t *nextchild;
    char label;
    char *data;
} TrieNode;

struct Dict_t
{
    TrieNode *root;
    TrieNode **array;
    size_t size;
    size_t nbkeys;
};
/**
 * @brief Récupère le noeud possèdant la donnée associée à la clé passée.
 *
 * @param d Trie
 * @param key Clé de la donnée
 * @return Node* NULL si la donnée ne se trouve pas dans le trie sinon le noeud
 */
static TrieNode *dictGet(Dict *d, const char *key);
/**
 * @brief Initialise le noeud avec ses membres.
 *
 * @param node Noeud à initialiser
 * @param children Le premier enfant du noeud
 * @param parent Le parent du noeud
 * @param nextchild Le prochain frère/soeur
 * @param key La clé associée au noeud
 * @param data La donnée que contient le noeud
 */
static void init_node(TrieNode *node, TrieNode *children, TrieNode *parent, TrieNode *nextchild, char key, char *data);
/**
 * @brief Ajoute un noeud au trie mais ne l'initialise pas et ne l'insère pas avec les autres noeuds.
 *        Le but de cette fonction est en vue de faciliter la méthode dictFree
 *
 * @param d Trie
 * @param node Noeud à ajouer
 */
static void append_node(Dict *d, TrieNode *node);
/**
 * @brief Recherche récursive du mot le plus long.
 *
 * @param children Noeud de départ
 * @param letters Lettres disponibles
 * @param longest Le plus long mot déjà trouvé en amont
 * @return const char* Le mot le plus long trouvé en chemin
 */
static const char *dictSearchLongest_rec(TrieNode *children, const char *letters, const char *longest);

static TrieNode *dictGet(Dict *d, const char *key)
{
    size_t index = 0;
    TrieNode *next_node = d->root;
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

static void init_node(TrieNode *node, TrieNode *children, TrieNode *parent, TrieNode *nextchild, char label, char *data)
{
    node->children = children;
    node->parent = parent;
    node->nextchild = nextchild;
    node->label = label;
    node->data = data;
}

static void append_node(Dict *d, TrieNode *node)
{
    if (d->nbkeys + 1 == d->size)
    {
        d->size = 2 * d->size; // On multiplie pour avoir une complexité linéaire
        d->array = realloc(d->array, d->size * sizeof(TrieNode *));
        if (!d->array)
            return;
    }
    d->array[d->nbkeys] = node;
    d->nbkeys++;
}

Dict *dictCreateEmpty()
{
    Dict *dict = malloc(sizeof(Dict));
    if (!dict)
        return NULL;
    TrieNode *root = malloc(sizeof(TrieNode));
    if (!root)
    {
        free(dict);
        return NULL;
    }
    init_node(root, NULL, NULL, NULL, '\0', NULL);

    dict->root = root;
    dict->size = 10;
    dict->array = calloc(dict->size, sizeof(TrieNode *));
    if (!dict->array)
    {
        free(dict);
        free(root);
        return NULL;
    }
    dict->nbkeys = 0;
    return dict;
}

size_t dictNbKeys(Dict *d)
{
    return d->nbkeys;
}

void dictFree(Dict *d)
{
    for (size_t i = 0; i < d->nbkeys; i++)
        free(d->array[i]);
    free(d->root);
    free(d->array);
    free(d);
}

int dictContains(Dict *d, const char *key)
{
    return dictSearch(d, key) != NULL;
}

void *dictSearch(Dict *d, const char *key)
{
    TrieNode *node = dictGet(d, key);
    return node ? node->data : NULL; // NULL si aucun mot n'a été trouvé
}

static const char *dictSearchLongest_rec(TrieNode *children, const char *letters, const char *longest)
{
    if (strcmp((char *)letters, "") == 0 || children == NULL) // Plus d'enfant ou de lettre à traiter
        return longest;
    TrieNode *child = children;
    char *subletters = NULL;
    const char *candidate = NULL;
    while (child) // On boucle sur tous les enfants
    {
        subletters = strchr(letters, child->label); // Renvoie une sous-chaîne à partir de laquelle on retrouve la lettre
        if (subletters)
        {
            if (child->data && strlen(child->data) > strlen(longest)) // On retrouve un mot et il est plus long
                longest = child->data;
            candidate = dictSearchLongest_rec(child->children, subletters + 1, longest); // On traite les enfants en avançant d'une lettre
            if (strlen(candidate) > strlen(longest))                                     // On a trouve un mot plus long
                longest = candidate;
        }
        child = child->nextchild;
    }
    return longest;
}

void *dictSearchLongest(Dict *d, const char *letters)
{
    const char *longest = dictSearchLongest_rec(d->root->children, letters, "");
    return strcmp((char *)longest, "") == 0 ? NULL : (char *)longest; // longest est vide si c'est le mot initial qui a démarré l'algorithme
}

void dictInsert(Dict *d, const char *key, void *data)
{
    TrieNode *node = dictGet(d, key);
    if (node)
        node->data = (char *)data;
    else
    {
        size_t index = 0;
        TrieNode *next_node = d->root, *prev_child = NULL, *parent_node = NULL, *new_node = NULL;
        while (key[index] != '\0') // On rajoute toutes les lettres de la clé dans le trie
        {
            parent_node = next_node;
            next_node = parent_node->children; // On regarde les prochaines lettres stockées
            while (next_node)                  // Max 26 tours pour chercher la lettre (nbre de char possibles)
            {
                if (next_node->label == key[index])
                    break;
                prev_child = next_node;
                next_node = next_node->nextchild;
            }
            if (!next_node) // Lettre non trouvée, on doit l'ajouter
            {
                new_node = malloc(sizeof(TrieNode));
                if (!new_node)
                    return;
                init_node(new_node, NULL, parent_node, NULL, key[index], NULL);
                if (parent_node->children) // Pas le premier enfant
                    prev_child->nextchild = new_node;
                else // Premier enfant
                    parent_node->children = new_node;
                append_node(d, new_node);
                next_node = new_node; // On prépare la prochaine itération
            }
            index++;
        }
        next_node->data = data; // Le dernier noeud doit forcément contenir la donnée
    }
}
