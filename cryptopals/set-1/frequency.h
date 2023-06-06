#pragma once 

// a struct representing an array of bigrams and a count
typedef struct {
    char** bigrams;
    int num_bigrams;
} bigram_array;

typedef struct {
    const char* key;
    float value;
} table_entry;

typedef struct {
    size_t size;
    table_entry* entries[];
} lookup_table;

char uppercase(char c);
char* uppercase_string(char* s);
double score_monogram(char* s);
bigram_array split_into_bigrams(char* s);
void free_bigram_array(bigram_array* ba);
double score_bigram(char* s);