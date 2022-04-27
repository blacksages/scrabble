#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "LinkedList.h"
#include "Scrabble.h"

#include "Dict.h"
#include "HashTable.c"

#define CUTOFF 31
static size_t _LETTER_COUNT[26] = {0};
/**
 * @brief Echange deux caractères d'un string.
 *
 * @param tab String
 * @param index_1 Indice du premier caractère
 * @param index_2 Indice du deuxième caractère
 */
static void swap_char(char *tab, int index_1, int index_2);
/**
 * @brief Vérifie si un mot peut être couvert par des lettres données par insertion.
 *        Les lettres qui couvrent le mot sont mises au début de letters.
 *
 * @param letters Lettres
 * @param word Mot à couvrir
 * @return true Si le mot peut être couvert par les lettres
 * @return false Si le mot ne peut pas être couvert par les lettres
 */
static bool match_insert(char *letters, const char *word);
/**
 * @brief Vérifie si un mot peut être couvert par des lettres données en comptant les lettres à l'aide de _LETTER_COUNT.
 *        On passe en revue chacun des lettres disponibles puis on regarde si le compte des lettres est suffisant pour couvrir le mot.
 *
 * @param letters Lettres
 * @param word Mot à couvrir
 * @return true Si le mot peut être couvert par les lettres
 * @return false Si le mot ne peut pas être couvert par les lettres
 */
static bool match_count(const char *letters, const char *word);
/**
 * @brief Vérifie si un mot peut être couvert par des lettres données.
 *        Si le nombre de lettre dépasse CUTOFF, on applique match_count, sinon on applique match_insert.
 *
 * @param letters Lettres
 * @param word Mot à couvrir
 * @return true Si le mot peut être couvert par les lettres
 * @return false Si le mot ne peut pas être couvert par les lettres
 */
static bool match(const char *letters, char *word);

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

    while (letters[index_l] != '\0' && word[index_w] != '\0') // On passe en revue toutes les lettres de chaque mot
    {
        if (letters[index_l] == word[index_w])
        {
            swap_char(letters, index_w, index_l);
            index_w++;
            index_l = index_w; // On a trouvé la n-ième lettre du mot, on recommence depuis le début + index_w
        }
        else
            index_l++; // On parcourt les lettres
    }

    return word[index_w] == '\0'; // Si on a couvert tout le mot, on est arrivé à la fin
}

static bool match_count(const char *letters, const char *word)
{
    memset(_LETTER_COUNT, 0, 26 * sizeof(int)); // On réinitialise les comptes
    size_t index = 0;
    while (letters[index] != '\0') // On compte les lettres données
    {
        _LETTER_COUNT[(letters[index] - 'a') % 26]++;
        index++;
    }
    index = 0;
    while (word[index] != '\0') // On soustrait les lettres du mot
    {
        if (_LETTER_COUNT[(word[index] - 'a') % 26] == 0) // On a plus de lettre disponible, on arrête
            break;
        else
        {
            _LETTER_COUNT[(word[index] - 'a') % 26]--;
            index++;
        }
    }
    return word[index] == '\0'; // Si on a couvert tout le mot, on est arrivé à la fin
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
    {
        printf("Allocation error in scrabbleCreateDict\n");
        return NULL;
    }

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
