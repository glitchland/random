
#include <stdio.h>
#include <string.h>

#include "frequency.h"
#include "utils.h"
#include "xor.h"

struct byte_array* xor_hex_str_with_byte_key(char* hex_str, unsigned char key) {
    struct byte_array* result = hex_str_to_byte_array(hex_str);
    for (int i = 0; i < result->size; i++) {
        result->bytes[i] = result->bytes[i] ^ key;
    }
    return result;
}

struct key_score_table* new_key_score_table(size_t size) {
    struct key_score_table* result = calloc(1, 
      sizeof(struct key_score_table) +
      (size * sizeof(struct key_score*)) // space for the pointers
    );
    
    result->size = size;
    for (int i = 0; i < size; i++) {
        result->entries[i] = calloc(1, sizeof(struct key_score));
        result->entries[i]->unencrypted_bytes = NULL;
        result->entries[i]->monogram_score = 0.0;
        result->entries[i]->bigram_score = 0.0;
        result->entries[i]->key = i;
    }

    return result;
}

void free_score_key_table(struct key_score_table* table) {
    for (int i = 0; i < table->size; i++) {
        free_key_score(table->entries[i]);
    }
    memset(table, 0, sizeof(struct key_score_table));
    free(table);
    table = NULL;
}


void free_key_score(struct key_score* ks) {
    if (ks->unencrypted_bytes != NULL) {
        free(ks->unencrypted_bytes);
        ks->unencrypted_bytes = NULL;
    }
    if (ks->encrypted_bytes != NULL) {
        free(ks->encrypted_bytes);
        ks->encrypted_bytes = NULL;
    }
    memset(ks, 0, sizeof(struct key_score));
    free(ks);
    ks = NULL;
}

char* copy_bytes(char* bytes) {
    char* dest = calloc(strlen(bytes) + 1, sizeof(char));

    size_t destSize = strlen(bytes) + 1;
    size_t srcLen = strlen(bytes);

    strncpy(dest, bytes, destSize);
    dest[destSize - 1] = '\0';

    return dest;
}

// given a key_score_table, find the key_score with the highest bigram score
struct key_score* select_best_bigram_score(struct key_score_table* table) {
    double top_score = 0.0;
    int top_score_index = 0;
    for (int i = 0; i < table->size; i++) {
        if (table->entries[i]->bigram_score > top_score) {
            top_score = table->entries[i]->bigram_score;
            top_score_index = i;
        }
    }

    struct key_score* result = calloc(1, sizeof(struct key_score));
    result->key = table->entries[top_score_index]->key;
    result->monogram_score = table->entries[top_score_index]->monogram_score;
    result->bigram_score = table->entries[top_score_index]->bigram_score;
    result->unencrypted_bytes = copy_bytes(
        table->entries[top_score_index]->unencrypted_bytes
    );
    result->encrypted_bytes = copy_bytes(
        table->entries[top_score_index]->encrypted_bytes
    );

    return result;
}

struct key_score* select_best_xor_key_score(char* hex_str) {
    struct key_score_table* table = new_key_score_table(256);

    // calculate the monogram and bigram scores for each possible key
    for (int i = 0; i < 256; i++) {
        struct byte_array* xored = xor_hex_str_with_byte_key(hex_str, i);
        char* xored_str = byte_array_to_str(xored);
        free_byte_array(xored);
        xored = NULL;

        table->entries[i]->key = i;
        table->entries[i]->monogram_score = score_monogram(xored_str);
        table->entries[i]->bigram_score = score_bigram(xored_str);
        table->entries[i]->unencrypted_bytes = copy_bytes(xored_str);
        table->entries[i]->encrypted_bytes = copy_bytes(hex_str);

        free(xored_str);
        xored_str = NULL;
    }

    // now select the best bigram key score entry from the table
    struct key_score* result = select_best_bigram_score(table);

    // this frees the unencrypted bytes, so that is why we copied it
    free_score_key_table(table);

    return result;
}

// this function prints out a key score 
void print_key_score(struct key_score* key_score) {
    if (key_score == NULL) {
        printf("top_score is NULL\n");
        return;
    }

    printf(" monogram score: %f \n bigram score: %f \n key: %1x\n ciphertext: %s\n plaintext: %s\n", 
        key_score->monogram_score, 
        key_score->bigram_score,        
        key_score->key,
        key_score->encrypted_bytes,
        key_score->unencrypted_bytes
    );
}
