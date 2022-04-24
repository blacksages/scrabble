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

//heapsort for strings
static void max_heapify_char(char *array, int i, int heap_size){
  int l = 2 * i + 1;
  int r = 2 *  i + 2;
  int largest = i;
  if(l < heap_size && array[l] > array[i])
    largest = l;
  /*
  else
    largest = i;
  */
  if(r < heap_size && array[r] > array[largest])
    largest = r;

  if(largest != i){
    swap_char(array, i, largest);
    max_heapify_char(array, largest, heap_size);
  }

}

static void build_max_heap_char(char *array, int array_size){
  int heap_size = array_size;
  int i;
  for(i = array_size / 2 - 1; i >= 0 ; i--)
    max_heapify_char(array, i, heap_size);
}

static void heapsort_char(char *array, int array_size){
  build_max_heap_char(array, array_size);
  int i;
  for(i = array_size - 1; i >= 0; i--){
    swap_char(array, i, 0);
    max_heapify_char(array, 0, i);
  }
}


////////////////////////

static bool does_b_contains_a(char *a, char *b, int size_a, int size_b){
	int index_a = 0;
	int index_b = 0;

	while(index_a < size_a && index_b < size_b){
		if(a[index_a] == b[index_b]){
			index_a++;
			index_b++;
		}
		else
			index_b++;
	}

	if(index_a >= size_a)
		return true;
	else
		return false;

}
//toutes les lettres du mots doivent être dans l'ensemble de lettre
//intersection de 2 ensembles
//intersection de 2 MULTIensembles
static bool match(const char* letters, char* word) {
    // à améliorer

		/*
    size_t nbLetters = strlen(letters);
    size_t lengthWord = strlen(word);

    if (lengthWord > nbLetters)
        return false;

    bool mask[nbLetters];
    for (size_t i = 0; i < nbLetters; i++)
        mask[i] = false;

    for (size_t i = 0; i < lengthWord; i++) {
        size_t j = 0;

        while (j < nbLetters && (mask[j] || letters[j] != word[i]))
            j++;

        if (j == nbLetters)
          return false;

        mask[j] = true;
    }

    return true;
		*/

	int len_letters = strlen(letters);
	int len_word = strlen(word);

	//Si le mot est plus long que l'ensemble de lettres, alors d'office il n'y a pas de match
	if(len_word > len_letters)
		return false;

	//créer une copie de l'ensemble de lettre que l'on peut trier;
	char *letters_copy = malloc(strlen(letters) * sizeof(char));

	for(int i = 0; i < len_letters; i++)
		letters_copy[i] = letters[i];
	///////////////////////////////////////////////////////////////

	//tri des 2 strings
	heapsort_char(word, len_word);
	heapsort_char(letters_copy, len_letters);
	///////////////////



	return does_b_contains_a(word, letters_copy, len_word, len_letters);


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
