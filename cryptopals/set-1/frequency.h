#pragma once 

// a struct representing an array of bigrams and a count
typedef struct {
    char** bigrams;
    int num_bigrams;
} bigram_array;

typedef struct {
    const char* key;
    double value;
} table_entry;

typedef struct {
    size_t size;
    table_entry* entries;
} lookup_table;

char uppercase(char c);
char* uppercase_string(char* s);

//monograms
lookup_table* get_monogram_table();
void free_monogram_table(lookup_table* table);
double score_monogram(char* s);

//bigrams
bigram_array str_to_bigrams(char* s);
void free_bigram_array(bigram_array* ba);
lookup_table* get_bigram_table();
void free_bigram_table(lookup_table* table);
double score_bigram(char* s);