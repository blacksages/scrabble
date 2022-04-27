#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Dict.h"
#include "LinkedList.h"
#include "Scrabble.h"


struct ScrabbleDict_t {
    Dict *dictionary;
};

////////////////////////////////////////////
//heapsort for strings
static void swap_char(char *tab, int index_1, int index_2){
  char tmp = tab[index_1];
  tab[index_1] = tab[index_2];
  tab[index_2] = tmp;
}

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
////////////////////////////////////////////


ScrabbleDict* scrabbleCreateDict(List* words) {
		//Creating a scrabble dictionary
    ScrabbleDict* scrabble_dictionnary = malloc(sizeof(ScrabbleDict));

		if(scrabble_dictionnary == NULL)
			return NULL;

		//Creating empty dictionnary
		Dict *dict = dictCreateEmpty();

		size_t len_word;
		size_t len_key;
		char *data;
		char *key;
		Node *node = llHead(words);

		while(node != NULL){
			//Recovering the word
			data = (char *)llData(node);

			//Creating a separate copy of the word, as we'll need one for the data
			//and one for the key.
			len_word = strlen(data);
			key = malloc(len_word * sizeof(char) + 1);

			if(key == NULL)
				return NULL;

			strcpy(key, data);

			//Sorting the key
			len_key = strlen(key);
			heapsort_char(key, len_key);

			//Inserting in the dictionnary
			dictInsert(dict, (const char*)key, (void *)data);

			//Iterating over the list
			node = llNext(node);
		}

		scrabble_dictionnary->dictionary = dict;
		return scrabble_dictionnary;
}

void scrabbleFreeDict(ScrabbleDict* sd) {
		dictFree(sd->dictionary);
    free(sd);
}

//file version
/*
static void subsets(const char *set, size_t set_size, size_t index, char *subset, size_t subset_size, FILE *output_file){
	if(index >= set_size){
		fprintf(output_file, "%s\n",subset );
	}
	else{
		//fprintf(stderr, "set : %s subset %s\n", set, subset);
		subsets(set, set_size, index + 1, subset, subset_size, output_file);
		char element = set[index];
		char new_subset[subset_size + 1];
		strcpy(new_subset, subset);
		//fprintf(stderr, "new subset %s\n",new_subset );
		strncat(new_subset, &element, 1);

		subsets(set, set_size, index + 1, new_subset, subset_size + 1, output_file);
	}
}
*/

//list version
static void subsets(const char *set, size_t set_size, size_t index, char *subset, size_t subset_size, List *output_list){
	if(index >= set_size){
		Node *node = llCreateNode(subset);
		llInsertLast(output_list, node);
	}
	else{
		//fprintf(stderr, "set : %s subset %s\n", set, subset);
		subsets(set, set_size, index + 1, subset, subset_size, output_list);
		char element = set[index];
		char *new_subset = malloc((subset_size + 1) * sizeof(char) + 1);
		strcpy(new_subset, subset);
		//fprintf(stderr, "new subset %s\n",new_subset );
		strncat(new_subset, &element, 1);

		subsets(set, set_size, index + 1, new_subset, subset_size + 1, output_list);
	}
}

char* scrabbleFindLongestWord(ScrabbleDict* sd, const char* letters) {
	//list output version
	//Declaring variables for subsets generation
	size_t len_letters = strlen(letters);
	List *output_list = llCreateEmpty();

	//Generating subsets
	subsets(letters, len_letters, 0, "", 0, output_list);

	//Declaring necessary variables
	Dict *dict = sd->dictionary;//pointer to the dictionary;
	char *key; // = malloc(len_letters * sizeof(char) + 1); // stocks the current key
	char * data; // stocks the current word associated to the key
	char * max_word = NULL; //stocks the biggest word found
	size_t max_size = 0; // stocks the size of the biggest word found
	size_t tmp; // stocks the size of the current word
	Node *subset = llHead(output_list);// stocks the node to the current subset

	//stop when the current node is empty
	while(subset != NULL){
		//Recover the key, which is a subset of the letters
		key = (char *)llData(subset);
		heapsort_char(key, strlen(key));
		//Lookup the dictionary
		data = (char *)dictSearch(dict, (const char*)key);

		//If we find a value associated to the key
		if(data != NULL){
			//Compute the length
			tmp = strlen(data);
			//If that length is strictly higher than the current max, it becomes the new max
			if(tmp > max_size){
				max_size = tmp;
				max_word = data;
			}
		}
		//Iterate to next node
		subset = llNext(subset);
	}

	//Free the now useless subsets list
	llFree(output_list);
	//free(key);
	//return the word;
	return max_word;
	//file output version
	/*
	//Declaring variables for subsets generation
	char *output_file_name = "subsets.txt";
	FILE *output_file = fopen(output_file_name, "w");
	size_t len_letters = strlen(letters);

	//Generating subsets
	subsets(letters, len_letters, 0, "", 0, output_file);

	//Switching to read mode
	fclose(output_file);
	output_file = fopen(output_file_name, "r");

	//Declaring necessary variables
	Dict *dict = sd->dictionary;//pointer to the dictionary;
	char key[len_letters]; // stocks the current key
	char * data; // stocks the current word associated to the key
	char * max_word = NULL; //stocks the biggest word found
	size_t max_size = 0; // stocks the size of the biggest word found
	size_t tmp; // stocks the size of the current word

	//
	while(fscanf(output_file, "%s", key) != EOF){
		//Recover the word associated tot he key
		data = (char *)dictSearch(dict, (const char*)key);

		//If there's a word associated to that key
		if(data != NULL){
			//Compute it's length
			tmp = strlen(data);
			//If it's bigger than the current max, register that word as the new max
			if(tmp > max_size){
				max_word = data;
				max_size = tmp;
			}
		}
	}

	fclose(output_file);
	return max_word;
	*/
}
