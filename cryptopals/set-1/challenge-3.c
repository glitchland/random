#include <stdio.h>
#include <stdbool.h>

#include "utils.h"
#include "frequency.h"

struct key_score {
    double monogram_score;
    double bigram_score;
    unsigned char key;
    char *unencrypted_bytes;
};

struct key_score_table {
    size_t size;
    struct key_score* entries[];
};


struct key_score_table* new_key_score_table(size_t size) {
    struct key_score_table* result = malloc(sizeof(struct key_score_table) + size * sizeof(struct key_score*));
    result->size = size;
    for (int i = 0; i < size; i++) {
        result->entries[i] = malloc(sizeof(struct key_score));
        result->entries[i]->monogram_score = 0.0;
        result->entries[i]->bigram_score = 0.0;
        result->entries[i]->key = i;
    }
    return result;
}

void free_score_key_table(struct key_score_table* table) {
    for (int i = 0; i < table->size; i++) {
        free(table->entries[i]->unencrypted_bytes);
        free(table->entries[i]);
        table->entries[i] = NULL;
    }
    free(table);
    table = NULL;
}

struct byte_array* xor_hex_str_with_byte_key(char* hex_str, unsigned char key) {
    struct byte_array* result = hex_str_to_byte_array(hex_str);
    for (int i = 0; i < result->size; i++) {
        result->bytes[i] = result->bytes[i] ^ key;
    }
    return result;
}

void main() {
    char *hex_str = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";

    struct key_score_table* table = new_key_score_table(256);

    // for every possible byte from 0x00 to 0xff, xor the hex string with the
    // byte, and then score the result
    for (int i = 0; i < 256; i++) {
        struct byte_array* xored = xor_hex_str_with_byte_key(hex_str, i);
        char* xored_str = byte_array_to_str(xored);
        table->entries[i]->key = i;
        table->entries[i]->monogram_score = score_monogram(xored_str);
        table->entries[i]->bigram_score = score_bigram(xored_str);
        table->entries[i]->unencrypted_bytes = xored_str;
        free_byte_array(xored);
        xored = NULL;
    }

    // find the top score and print it
    double top_score = 0.0;
    int top_score_index = 0;
    for (int i = 0; i < table->size; i++) {
        if (table->entries[i]->bigram_score > top_score) {
            top_score = table->entries[i]->bigram_score;
            top_score_index = i;
        }
    }

    printf("monogram score:%f bigram score: %f key:%x plaintext:%s\n", 
        table->entries[top_score_index]->monogram_score, 
        table->entries[top_score_index]->bigram_score,
        table->entries[top_score_index]->key,
        table->entries[top_score_index]->unencrypted_bytes
    );

    free_score_key_table(table);
}