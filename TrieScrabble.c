#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Dict.h"
#include "LinkedList.h"
#include "Scrabble.h"

struct ScrabbleDict_t
{
    Dict *dict;
};

////////////////////////////////////////////
// Heapsort issu du projet 1
static void swap_char(char *tab, int index_1, int index_2)
{
    char tmp = tab[index_1];
    tab[index_1] = tab[index_2];
    tab[index_2] = tmp;
}

static void max_heapify_char(char *array, int i, int heap_size)
{
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    int largest = i;
    if (l < heap_size && array[l] > array[i])
        largest = l;
    /*
    else
      largest = i;
    */
    if (r < heap_size && array[r] > array[largest])
        largest = r;

    if (largest != i)
    {
        swap_char(array, i, largest);
        max_heapify_char(array, largest, heap_size);
    }
}

static void build_max_heap_char(char *array, int array_size)
{
    int heap_size = array_size;
    int i;
    for (i = array_size / 2 - 1; i >= 0; i--)
        max_heapify_char(array, i, heap_size);
}

static void heapsort_char(char *array, int array_size)
{
    build_max_heap_char(array, array_size);
    int i;
    for (i = array_size - 1; i >= 0; i--)
    {
        swap_char(array, i, 0);
        max_heapify_char(array, 0, i);
    }
}
////////////////////////////////////////////

ScrabbleDict *scrabbleCreateDict(List *words)
{
    Dict *dict = dictCreateEmpty();
    Node *node = llHead(words);
    char *data = NULL;
    char *keys;
    size_t *lens_including_null = calloc(llLength(words), sizeof(size_t));
    if (!lens_including_null)
    {
        printf("Allocation error in scrabbleCreateDict!\n");
        return NULL;
    }
    size_t total_len = 0, offset = 0;
    size_t i = 0;
    while (node) // On cherche à avoir toutes les clés en une fois pour éviter des malloc/free en boucle
    {
        lens_including_null[i] = strlen((char *)llData(node)) + 1;
        total_len += lens_including_null[i];
        node = llNext(node);
        i++;
    }
    keys = calloc(total_len, sizeof(char));
    if (!keys)
    {
        printf("Allocation error in scrabbleCreateDict!\n");
        return NULL;
    }
    node = llHead(words);
    i = 0;
    while (node) // Insertion des couples clé/donnée
    {
        data = (char *)llData(node);
        strcpy(keys + offset, data);
        heapsort_char(keys + offset, lens_including_null[i] - 1); // Tri par heapsort de la clé
        dictInsert(dict, keys + offset, data);
        node = llNext(node);
        offset += lens_including_null[i];
        i++;
    }
    free(lens_including_null); // Au final 2 free et 2 calloc plutôt de que n free et n malloc
    free(keys);
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
}

char *scrabbleFindLongestWord(ScrabbleDict *sd, const char *letters)
{
    size_t len = strlen(letters);
    char *sorted_l = malloc((len + 1) * sizeof(char));
    if (!sorted_l)
    {
        printf("Allocation error in scrabbleFindLongestWord!\n");
        return NULL;
    }
    strcpy(sorted_l, letters);
    heapsort_char(sorted_l, len); // Tri des lettres par heapsort
    char *longest = dictSearchLongest(sd->dict, sorted_l);
    free(sorted_l);
    return longest;
}
