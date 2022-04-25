#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "LinkedList.h"
#include "Scrabble.h"


struct ScrabbleDict_t {
    List* words;
};

static void swap_char(char *tab, int index_1, int index_2){
  char tmp = tab[index_1];
  tab[index_1] = tab[index_2];
  tab[index_2] = tmp;
}

//toutes les lettres du mots doivent être dans l'ensemble de lettre
//intersection de 2 ensembles
//intersection de 2 MULTIensembles
static bool match(const char* letters, char* word) {
	if(strlen(word) > strlen(letters))
		return false;
  
  int index_l = 0, index_w = 0, offset = 0;
	
	while(letters[index_l] != '\0' && word[index_w] != '\0'){
		if(letters[index_l] == word[index_w]){
      swap_char(letters, offset++, index_l);
      index_l = offset;
		}
		index_w++;
	}

	return letters[index_l] == '\0';
}



ScrabbleDict* scrabbleCreateDict(List* words) {
    ScrabbleDict* sd = malloc(sizeof(ScrabbleDict));
    if (!sd)
        return NULL;

    sd->words = words;

    return sd;
}

void scrabbleFreeDict(ScrabbleDict* sd) {
    free(sd);
}

char* scrabbleFindLongestWord(ScrabbleDict* dict, const char* letters) {
    char* longestWord = NULL;
    size_t longestLength = 0;

    for (Node* p = llHead(dict->words); p != NULL; p = llNext(p)) {
        char* word = llData(p);

        if (match(letters, word)) {
            size_t length = strlen(word);

            if (length > longestLength) {
                longestWord = word;
                longestLength = length;
            }
        }
    }

    return longestWord;
}
