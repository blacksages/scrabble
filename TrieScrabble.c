#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Dict.h"
#include "LinkedList.h"
#include "Scrabble.h"

extern char *strdup(const char *); // strdup not declared in C99

struct ScrabbleDict_t
{
    Dict *dict;
};

////////////////////////////////////////////
// Heapsort issu du projet 1
/**
 * @brief Echange deux caractères dans une chaîne de caractères.
 * 
 * @param str Chaîne de caractères
 * @param index_1 Indice du premier caractère
 * @param index_2 Indice du deuxième caractère
 */
static void swap_char(char *str, int index_1, int index_2);
/**
 * @brief Remonte d'un cran la plus haute valeur dans le tas à partir de la position i en comparant
 * 		  avec les valeurs à droite et à gauche.
 * 
 * @param str Chaîne de caractère servant de tas
 * @param i Position de départ
 * @param heap_size Taille du tas
 */
static void max_heapify_char(char *str, int i, int heap_size);
/**
 * @brief Construit le tas de départ pour avoir la plus haut valeur en haut.
 * 
 * @param str Chaîne de caractère servant de tas
 */
static void build_max_heap_char(char *str);
/**
 * @brief Trie par tas une chaîne de caractères dans l'ordre lexicographique.
 * 
 * @param str Chaîne de caractère servant de tas
 */
static void heapsort_char(char *str);

static void swap_char(char *str, int index_1, int index_2)
{
	char tmp = str[index_1];
	str[index_1] = str[index_2];
	str[index_2] = tmp;
}

static void max_heapify_char(char *str, int i, int heap_size)
{
	int l = 2 * i + 1;
	int r = 2 * i + 2;
	int largest = i;
	if (l < heap_size && str[l] > str[i])
		largest = l;
	if (r < heap_size && str[r] > str[largest])
		largest = r;

	if (largest != i)
	{
		swap_char(str, i, largest);
		max_heapify_char(str, largest, heap_size);
	}
}

static void build_max_heap_char(char *str)
{
	int heap_size = strlen(str);
	for (int i = heap_size / 2 - 1; i >= 0; i--)
		max_heapify_char(str, i, heap_size);
}

static void heapsort_char(char *str)
{
	build_max_heap_char(str);
	for (int i = strlen(str) - 1; i >= 0; i--)
	{
		swap_char(str, i, 0);
		max_heapify_char(str, 0, i);
	}
}
////////////////////////////////////////////

ScrabbleDict *scrabbleCreateDict(List *words)
{
    Dict *dict = dictCreateEmpty();
    Node *node = llHead(words);
    char *data = NULL;
    char *key = NULL;
    node = llHead(words);
    while (node) // Insertion des couples clé/donnée
    {
        data = (char *)llData(node);
        key = strdup(data);
        if (!key)
        {
            printf("Allocation error in scrabbleCreateDict!\n");
            return NULL;
        }
        heapsort_char(key); // Tri par heapsort de la clé
        dictInsert(dict, key, data);
        free(key);
        node = llNext(node);
    }
    ScrabbleDict *sd = malloc(sizeof(ScrabbleDict));
    if (!sd)
    {
        printf("Allocation error in scrabbleCreateDict!\n");
        return NULL;
    }
    sd->dict = dict;
    return sd;
}

void scrabbleFreeDict(ScrabbleDict *sd)
{
    dictFree(sd->dict);
    free(sd);
}

char *scrabbleFindLongestWord(ScrabbleDict *sd, const char *letters)
{
    char *sorted_l = strdup(letters);
    if (!sorted_l)
    {
        printf("Allocation error in scrabbleFindLongestWord!\n");
        return NULL;
    }
    heapsort_char(sorted_l); // Tri des lettres par heapsort
    char *longest = dictSearchLongest(sd->dict, sorted_l);
    free(sorted_l);
    return longest;
}
