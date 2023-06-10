#pragma once

#include <stdio.h>

struct key_score {
    double monogram_score;
    double bigram_score;
    unsigned char key;
    char *unencrypted_bytes;
    char *encrypted_bytes;
};

struct key_score_table {
    size_t size;
    struct key_score* entries[];
};


struct key_score_table* new_key_score_table(size_t size);
void free_score_key_table(struct key_score_table* table);
void free_key_score(struct key_score* score);
struct byte_array* xor_hex_str_with_byte_key(char* hex_str, unsigned char key);
void xor_with_repeating_key(struct byte_array* plaintext, struct byte_array* key);
struct key_score* select_best_xor_key_score(char* hex_str);
struct key_score* select_best_bigram_score(struct key_score_table* table);
void print_key_score(struct key_score* key_score);