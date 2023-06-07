#include <stdlib.h>
#include <string.h>

#include "frequency.h"

lookup_table* get_monogram_table() {
    static table_entry monogram_entries[] = {
        {"A", 8.55},
        {"B", 1.60},
        {"C", 3.16},
        {"D", 3.87},
        {"E", 12.10},
        {"F", 2.18},
        {"G", 2.09},
        {"H", 4.96},
        {"I", 7.33},
        {"J", 0.22},
        {"K", 0.81},
        {"L", 4.21},
        {"M", 2.53},
        {"N", 7.17},
        {"O", 7.47},
        {"P", 2.07},
        {"Q", 0.10},
        {"R", 6.33},
        {"S", 6.73},
        {"T", 8.94},
        {"U", 2.68},
        {"V", 1.06},
        {"W", 1.83},
        {"X", 0.19},
        {"Y", 1.72},
        {"Z", 0.11}
    };

    lookup_table* table = (lookup_table*)malloc(sizeof(lookup_table));
    table->size = sizeof(monogram_entries) / sizeof(table_entry);
    table->entries = monogram_entries;

    return table;
}

// given a monogram table, free all of the memory safely
void free_monogram_table(lookup_table* table) {
    free(table);
    table = NULL;
}

lookup_table* get_bigram_table() {
    static table_entry bigram_entries[] = {
        {"TH", 3.56},
        {"HE", 3.07},
        {"IN", 2.43},
        {"ER", 2.05},
        {"AN", 1.99},
        {"RE", 1.85},
        {"ES", 1.75},
        {"ON", 1.73},
        {"ST", 1.66},
        {"NT", 1.61},
        {"EN", 1.57},
        {"AT", 1.56},
        {"ED", 1.49},
        {"ND", 1.46},
        {"TO", 1.46},
        {"OR", 1.43},
        {"EA", 1.42},
        {"TI", 1.34},
        {"AR", 1.34},
        {"TE", 1.28},
        {"NG", 1.28},
        {"AL", 1.28},
        {"IT", 1.26},
        {"AS", 1.21},
        {"IS", 1.21},
        {"HA", 1.14},
        {"ET", 1.13},
        {"SE", 1.11},
        {"OU", 1.10},
        {"OF", 1.09},
        {"LE", 1.08},
        {"SA", 1.08},
        {"VE", 1.05},
        {"RO", 1.01},
        {"RA", 0.99},
        {"RI", 0.99},
        {"HI", 0.98},
        {"NE", 0.95},
        {"ME", 0.94},
        {"DE", 0.94},
        {"CO", 0.91},
        {"TA", 0.90},
        {"EC", 0.88},
        {"SI", 0.88},
        {"LL", 0.85},
        {"SO", 0.85},
        {"NA", 0.84},
        {"LI", 0.83},
        {"LA", 0.82},
        {"EL", 0.80},
        {"MA", 0.79},
        {"DI", 0.78},
        {"IC", 0.77},
        {"RT", 0.77},
        {"NS", 0.77},
        {"OM", 0.76},
        {"CH", 0.76},
        {"OT", 0.75},
        {"CA", 0.74},
    };

    lookup_table* table = (lookup_table*)malloc(sizeof(lookup_table));
    table->size = sizeof(bigram_entries) / sizeof(table_entry);
    table->entries = bigram_entries;

    return table;
}

// given a monogram table, free all of the memory safely
void free_bigram_table(lookup_table* table) {
    free(table);
    table = NULL;
}

// return the uppercase version of a character
char uppercase(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - 32;
    }
    return c;
}

// return an uppercase version of a string 
char* uppercase_string(char* s) {
    char* result = malloc(strlen(s) + 1);
    for (int i = 0; i < strlen(s); i++) {
        result[i] = uppercase(s[i]);
    }
    result[strlen(s)] = '\0';
    return result;
}

// Monograms
double score_monogram(char* s) {
    double score = 0.0;
    char* uppercase_s = uppercase_string(s);

    lookup_table *monogram_table = get_monogram_table();

    for (int i = 0; i < strlen(uppercase_s); i++) {
        // match the character to the monogram table entry
        for (int j = 0; j < monogram_table->size; j++) {
            // read the table entry at this index
            if (uppercase_s[i] == *monogram_table->entries[j].key) {
                score += monogram_table->entries[j].value;
            }
        }
    }

    free_monogram_table(monogram_table);
    free(uppercase_s);
    uppercase_s = NULL;

    return score;
}

// Bigrams
bigram_array str_to_bigrams(char* s) {
    bigram_array result;
    result.num_bigrams = strlen(s) / 2;
    result.bigrams = malloc(result.num_bigrams * sizeof(char*));
    for (int i = 0; i < result.num_bigrams; i++) {
        result.bigrams[i] = malloc(3 * sizeof(char));
        result.bigrams[i][0] = s[2 * i];
        result.bigrams[i][1] = s[2 * i + 1];
        result.bigrams[i][2] = '\0';
    }
    return result;
}

void free_bigram_array(bigram_array* ba) {
    for (int i = 0; i < ba->num_bigrams; i++) {
        free(ba->bigrams[i]);
        ba->bigrams[i] = NULL;
    }
    free(ba->bigrams);
    ba->bigrams = NULL;
}

double score_bigram(char* s) {
    double score = 0.0;
    char* uppercase_s = uppercase_string(s);
    bigram_array bigrams = str_to_bigrams(uppercase_s);

    lookup_table *bigram_table = get_bigram_table();

    for (int i = 0; i < bigrams.num_bigrams; i++) {
        // match the bigram to the bigram table entry
        for (int j = 0; j < bigram_table->size; j++) {
            if (strcmp(bigrams.bigrams[i], bigram_table->entries[j].key) == 0) {
                score += bigram_table->entries[j].value;
            }
        }
    }

    free(uppercase_s);
    return score;
}