#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "LinkedList.h"
#include "Scrabble.h"

#include "Dict.h"
#include "HashTable.c"

#define CUTOFF 31

static void swap_char(char *tab, int index_1, int index_2);
static bool match_insert(char *letters, const char *word);
static bool match_count(const char *letters, const char *word);
static bool match(const char *letters, char *word);
static size_t _LETTER_COUNT[26] = {0};
struct ScrabbleDict_t
{
    List *words;
};

static void swap_char(char *tab, int index_1, int index_2)
{
    char tmp = tab[index_1];
    tab[index_1] = tab[index_2];
    tab[index_2] = tmp;
}

static bool match_insert(char *letters, const char *word)
{
    int index_l = 0, index_w = 0;

    while (letters[index_l] != '\0' && word[index_w] != '\0')
    {
        if (letters[index_l] == word[index_w])
        {
            swap_char(letters, index_w, index_l);
            index_w++;
            index_l = index_w;
        }
        index_l++;
    }

    return word[index_w] == '\0';
}

static bool match_count(const char *letters, const char *word)
{
    memset(_LETTER_COUNT, 0, 26 * sizeof(int));
    size_t index = 0;
    while (letters[index] != '\0')
    {
        _LETTER_COUNT[(letters[index] - 'a') % 26]++;
        index++;
    }
    index = 0;
    while (word[index] != '\0')
    {
        if (_LETTER_COUNT[(word[index] - 'a') % 26] == 0)
            break;
        else
        {
            _LETTER_COUNT[(word[index] - 'a') % 26]--;
            index++;
        }
    }
    return word[index] == '\0';
}

// toutes les lettres du mots doivent être dans l'ensemble de lettre
// intersection de 2 ensembles
// intersection de 2 MULTIensembles
static bool match(const char *letters, char *word)
{
    if (strlen(word) > strlen(letters))
        return false;
    if (strlen(letters) < CUTOFF)
        return match_insert(letters, word);
    else
        return match_count(letters, word);
}

ScrabbleDict *scrabbleCreateDict(List *words)
{
    ScrabbleDict *sd = malloc(sizeof(ScrabbleDict));
    if (!sd)
        return NULL;

    sd->words = words;

    return sd;
}

void scrabbleFreeDict(ScrabbleDict *sd)
{
    free(sd);
}

char *scrabbleFindLongestWord(ScrabbleDict *dict, const char *letters)
{
    char *longestWord = NULL;
    size_t longestLength = 0;

    for (Node *p = llHead(dict->words); p != NULL; p = llNext(p))
    {
        char *word = llData(p);

        if (match(letters, word))
        {
            size_t length = strlen(word);

            if (length > longestLength)
            {
                longestWord = word;
                longestLength = length;
            }
        }
    }

    return longestWord;
}
