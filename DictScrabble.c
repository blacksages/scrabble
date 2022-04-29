#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Dict.h"
#include "LinkedList.h"
#include "Scrabble.h"

extern char *strdup(const char *); // strdup not declared in C99

struct ScrabbleDict_t
{
	Dict *dictionary;
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
/**
 * @brief Ajoute les sous-ensembles d'une chaîne dans une liste liée de manière récursive.
 *
 * @param set Chaîne dont on cherche les sous-ensembles
 * @param set_size Taille de la chaîne, typiquement strlen(set)
 * @param index Indice à partir du quel on compose les sous-ensemble, laissez à 1
 * @param subset Sous-ensemble de départ, typiquement malloc d'une chaîne vide
 * @param subset_size Taille du sous-ensemble
 * @param output_list Liste qui accueille les sous-ensembles
 */
static void subsets(const char *set, size_t set_size, size_t index, char *subset, size_t subset_size, List *output_list);

ScrabbleDict *scrabbleCreateDict(List *words)
{
	// Création d'un dictionnaire de scrabble
	ScrabbleDict *scrabble_dictionnary = malloc(sizeof(ScrabbleDict));

	if (scrabble_dictionnary == NULL)
		return NULL;

	// Dictionnaire initial vide
	Dict *dict = dictCreateEmpty();

	char *data;
	char *key;
	Node *node = llHead(words);

	while (node != NULL)
	{
		// Récupération du mot
		data = (char *)llData(node);
		// Crée une copie du mot puisqu'on en a besoin pour la clé en plus de la donnée
		key = strdup(data);
		if (!key)
			return NULL;
		// Trie de la clé
		heapsort_char(key);
		// Insertion dans le dictionnaire
		dictInsert(dict, key, (void *)data);
		// Plus d'utilité pour la clé
		free(key);
		// Prochaine itération
		node = llNext(node);
	}

	scrabble_dictionnary->dictionary = dict;
	return scrabble_dictionnary;
}

void scrabbleFreeDict(ScrabbleDict *sd)
{
	dictFree(sd->dictionary);
	free(sd);
}

static void subsets(const char *set, size_t set_size, size_t index, char *subset, size_t subset_size, List *output_list)
{
	if (index >= set_size)
	{
		Node *node = llCreateNode(subset);
		llInsertLast(output_list, node);
	}
	else
	{
		subsets(set, set_size, index + 1, subset, subset_size, output_list);
		char element = set[index];
		char *new_subset = malloc((subset_size + 1) * sizeof(char) + 1);
		strcpy(new_subset, subset);
		strncat(new_subset, &element, 1);

		subsets(set, set_size, index + 1, new_subset, subset_size + 1, output_list);
	}
}

char *scrabbleFindLongestWord(ScrabbleDict *sd, const char *letters)
{
	// Déclaration des variables pour la génération des sous-ensembles
	size_t len_letters = strlen(letters);
	List *output_list = llCreateEmpty();
	heapsort_char((char *)letters);
	// Génération des sous-ensembles
	// Initialise à 0 ou '\0' le char alloué qui sera utilisé pour le premier sous-ensemble
	char *empty_subset = calloc(1, sizeof(char));
	subsets(letters, len_letters, 0, empty_subset, 0, output_list);

	// Déclaration des variables nécessaires
	Dict *dict = sd->dictionary;
	char *key;
	char *data;
	char *max_word = NULL;
	size_t max_size = 0;
	size_t tmp;
	Node *subset = llHead(output_list);
	// S'arrête quand il n'y a plus de noeud
	while (subset != NULL)
	{
		key = (char *)llData(subset);
		data = (char *)dictSearch(dict, (const char *)key);

		if (data != NULL) // On a trouvé une valeur associée à la clé
		{
			tmp = strlen(data);
			// Si la longueur est strictement plus grande que l'actuel max, elle devient le nouveau max
			if (tmp > max_size)
			{
				max_size = tmp;
				max_word = data;
			}
		}
		// Prochaine itération
		subset = llNext(subset);
	}
	llFreeData(output_list);
	return max_word; // NULL si aucun sous-ensemble n'a été trouvé dans le dictionnaire
}