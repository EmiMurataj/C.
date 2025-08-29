#include <assert.h>
#include <dict.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wordle.h>

Trie *generateDict(char *filename, int k, char *selected1, char *selected2) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        exit(EXIT_FAILURE);
    }
    Trie *trie = create();        // we create the trie
    int initial_capacity = 2000;  // we initialise a capacity aka the number of words we assume that our dictionary has
    int index = 0;                // initialise the index of the array
    char **dictionary_words = malloc(initial_capacity * sizeof(char *));  // allocate memory for the array
    if (dictionary_words == NULL) {
        exit(EXIT_FAILURE);
    }
    char buf[256];
    while (fgets(buf, sizeof(buf), f)) {
        if (buf[k] == '\n') {
            buf[k] = '\0';
        }
        if (index == initial_capacity) {
            initial_capacity *= 2;
            dictionary_words = realloc(dictionary_words, initial_capacity * sizeof(char *));
            if (dictionary_words == NULL) {
                exit(EXIT_FAILURE);
            }
        }
        dictionary_words[index] = malloc(k + 1);
        strcpy(dictionary_words[index], buf);
        index++;
    }
    fclose(f);
    assert(index > 0);
    int idx1 = (int)(drand48() * index);
    strcpy(selected1, dictionary_words[idx1]);

    if (selected2 && index > 1) {
        int idx2;
        do {
            idx2 = (int)(drand48() * index);
        } while (idx2 == idx1);
        strcpy(selected2, dictionary_words[idx2]);
    }
    for (int i = 0; i < index; i++) {
        insert(trie, dictionary_words[i]);
        free(dictionary_words[i]);
    }
    free(dictionary_words);
    return trie;
}
char *guess(Trie *dict, int k) {
    char buf[256];
    int valid = 0;
    int first_try = 1;
    while (!valid) {
        if (first_try) {
            printf("Please input your guess: ");
            first_try = 0;
        } else {
            printf("Invalid word. Try again: ");
        }
        if (!fgets(buf, sizeof(buf), stdin)) {
            exit(EXIT_FAILURE);
        }
        if (buf[k] == '\n') {
            buf[k] = '\0';
        }
        for (int i = 0; i < k; i++) {
            if (buf[i] >= 'A' && buf[i] <= 'Z') {
                buf[i] = buf[i] - 'A' + 'a';
            }
        }
        if ((int)strlen(buf) != k) {
            valid = 0;
            continue;
        }
        valid = 1;
        for (int i = 0; i < k; i++) {
            if (buf[i] < 'a' || buf[i] > 'z') {
                valid = 0;
                break;
            }
        }
        if (dict != NULL && valid) {  // mund ta heqesh validin ktu se ndoshta ste duhet
            if (!lookup(dict, buf)) {
                valid = 0;
            }
        }
    }

    return strdup(buf);
}
feedback_result *getFeedback(char *guess, char *word1, char *word2, int k) {
    feedback_result *result = malloc(sizeof(feedback_result) * k);
    if (!result) return NULL;
    int used_word[k];
    for (int i = 0; i < k; i++) {
        result[i] = WRONG;
        used_word[i] = 0;
    }
    for (int i = 0; i < k; i++) {
        if (guess[i] == word1[i]) {
            result[i] = CORRECT;
            used_word[i] = 1;
        }
    }  // Your algorithm should run in time (𝑘)∗ for a word of length 𝑘.  havent done this
    for (int i = 0; i < k; i++) {
        if (result[i] == CORRECT) continue;
        for (int j = 0; j < k; j++) {
            if (!used_word[j] && guess[i] == word1[j]) {
                result[i] = WRONGPOS;
                used_word[j] = 1;
                break;
            }
        }
    }
    if (!word2) {
    } else {
    }
    return result;
}
void printFeedback(feedback_result *feedback, int k) {
    printf("Result: ");
    for (int i = 0; i < k; i++) {
        if (feedback[i] == CORRECT) {
            printf("🟩");  // kto mund ti ndrrosh me ngjyrat perkatese //green🟩
        } else if (feedback[i] == WRONGPOS) {
            printf("🟨");  // yellow
        } else if (feedback[i] == WRONG) {
            printf("⬛");  // black
        } else if (feedback[i] == QUANTUMCORRECT) {
            printf("🟢");
        } else if (feedback[i] == QUANTUMWRONGPOS) {
            printf("🟡");
        }
    }
    printf("\n");  // ends with a newline
}
bool checkWin(feedback_result *feedback, int k) {
    for (int i = 0; i < k; i++) {
        if (feedback[i] != CORRECT) {
            return false;  // As soon as one is NOT correct, player hasn't won
        }
    }
    return true;  // All feedback are CORRECT, so player won
}
