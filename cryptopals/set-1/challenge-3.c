#include <stdio.h>
#include <stdbool.h>

#include "utils.h"
#include "frequency.h"

// a struct containing a double score and a byte key
struct score_key {
    double score;
    unsigned char key;
};

// a table containing a user specified number of score_key structs
struct score_key_table {
    size_t size;
    struct score_key* entries[];
};

// return a score_key table with one score_key entry per possible byte value
// and a double score of 0.0
struct score_key_table* new_score_key_table(size_t size) {
    struct score_key_table* result = malloc(sizeof(struct score_key_table) + size * sizeof(struct score_key*));
    result->size = size;
    for (int i = 0; i < size; i++) {
        result->entries[i] = malloc(sizeof(struct score_key));
        result->entries[i]->score = 0.0;
        result->entries[i]->key = i;
    }
    return result;
}

// free a score_key table
void free_score_key_table(struct score_key_table* table) {
    for (int i = 0; i < table->size; i++) {
        free(table->entries[i]);
        table->entries[i] = NULL;
    }
    free(table);
    table = NULL;
}

// given a hex string and a byte key, xor the string with the key and return
// the result as a byte array
struct byte_array* xor_hex_str_with_byte_key(char* hex_str, unsigned char key) {
    struct byte_array* result = hex_str_to_byte_array(hex_str);
    for (int i = 0; i < result->size; i++) {
        result->bytes[i] = result->bytes[i] ^ key;
    }
    return result;
}

void main() {
    char *hex_str = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";

    struct score_key_table* table = new_score_key_table(256);

    // for every possible byte from 0x00 to 0xff, xor the hex string with the
    // byte, and then score the result
    for (int i = 0; i < 256; i++) {
        struct byte_array* xored = xor_hex_str_with_byte_key(hex_str, i);
        char* xored_str = byte_array_to_str(xored);
        table->entries[i]->key = i;
        table->entries[i]->score = score_monogram(xored_str);

        free(xored_str);
        xored_str = NULL;
        free_byte_array(xored);
        xored = NULL;
    }
}