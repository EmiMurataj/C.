#define Letters 26
#include <stdbool.h>
#include <stdlib.h>

#include "dict.h"

struct Trie {
    struct Trie *children[Letters];
    bool word_end;
};

Trie *create() {
    Trie *node = malloc(sizeof(Trie));
    if (!node) return NULL;
    for (int i = 0; i < Letters; i++) {
        node->children[i] = NULL;
    }
    node->word_end = false;
    return node;
}

void insert(Trie *dict, char *word) {
    if (!dict || !word) return;
    Trie *current = dict;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (index < 0 || index >= Letters) continue;  // skip invalid chars
        if (current->children[index] == NULL) {
            current->children[index] = create();
            if (!current->children[index]) return;  // malloc failure
        }
        current = current->children[index];
    }
    current->word_end = true;
}

bool lookup(Trie *dict, char *word) {
    if (!dict || !word) return false;
    Trie *current = dict;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (index < 0 || index >= Letters) return false;
        if (current->children[index] == NULL) return false;
        current = current->children[index];
    }
    return current->word_end;
}

void destroy(Trie *dict) {
    if (!dict) return;
    for (int i = 0; i < Letters; i++) {
        destroy(dict->children[i]);
    }
    free(dict);
}
