#include <stdlib.h>
#include <string.h>

#include "Dict.h"


struct Dict_t {
    // à compléter
};


Dict* dictCreateEmpty() {
    // à compléter
}

size_t dictNbKeys(Dict* d) {
    // à compléter
}

void dictFree(Dict* d) {
    // à compléter
}

int dictContains(Dict* d, const char* key) {
    // à compléter
}

void* dictSearch(Dict* d, const char* key) {
    // à compléter
}

void* dictSearchLongest(Dict* d, const char* letters) {
    // à compléter
}

void dictInsert(Dict* d, const char* key, void* data) {
    // à compléter
}
