#include <stdlib.h>
#include <string.h>

#include "frequency.h"

// monogram frequency table
table_entry monogram_lookup_table[] = {
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
    {"Z", 0.11},
};

lookup_table monogram_table = {
    .size = 26,
    .entries = monogram_lookup_table
};

// bigram frequency table, top 30
table_entry bigram_lookup_table[] = {
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
};

lookup_table bigram_table = {
    .size = 30,
    .entries = bigram_lookup_table
};

// trigram frequency table, top 30
table_entry trigram_lookup_table[] = {
    {"THE", 1.81},
    {"AND", 0.73},
    {"ING", 0.72},
    {"ENT", 0.42},
    {"ION", 0.42},
    {"HER", 0.36},
    {"FOR", 0.34},
    {"THA", 0.33},
    {"NTH", 0.33},
    {"INT", 0.32},
    {"ERE", 0.31},
    {"TIO", 0.31},
    {"TER", 0.30},
    {"EST", 0.28},
    {"ERS", 0.28},
    {"ATI", 0.26},
    {"HAT", 0.26},
    {"ATE", 0.25},
    {"ALL", 0.25},
    {"ETH", 0.24},
    {"HES", 0.24},
    {"VER", 0.24},
    {"HIS", 0.24},
    {"OFT", 0.24},
    {"ITH", 0.24},
    {"FTH", 0.24},
    {"STH", 0.24},
    {"OTH", 0.24},
    {"RES", 0.23},
    {"ONT", 0.23},
};

lookup_table trigram_table = {
    .size = 30,
    .entries = trigram_lookup_table
};

// quadgram frequency table, top 30
table_entry quadgram_lookup_table[] = {
    {"TION", 0.31},
    {"OTHE", 0.27},
    {"THAT", 0.24},
    {"THE_", 0.24},
    {"OF_T", 0.21},
    {"TO_T", 0.20},
    {"IN_T", 0.19},
    {"THES", 0.19},
    {"WITH", 0.18},
    {"ATIO", 0.18},
    {"FROM", 0.17},
    {"INTH", 0.17},
    {"THIS", 0.17},
    {"HAVE", 0.16},
    {"THEM", 0.16},
    {"THEC", 0.16},
    {"THEP", 0.16},
    {"THEI", 0.16},
    {"THEB", 0.16},
    {"THER", 0.16},
    {"THEA", 0.16},
    {"THEL", 0.16},
    {"THEF", 0.16},
    {"THEG", 0.16},
    {"THEH", 0.16},
    {"THEO", 0.16},
    {"THEW", 0.16},
    {"THEV", 0.16},
    {"THEU", 0.16},
    {"THEY", 0.16},
};

lookup_table quadgram_table = {
    .size = 30,
    .entries = quadgram_lookup_table
};

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

// given a string, convert the string to uppercase, and then calculate a
// score as a double, based on lookups into monogram frequencies
double score_monogram(char* s) {
    double score = 0.0;
    char* uppercase_s = uppercase_string(s);
    for (int i = 0; i < strlen(uppercase_s); i++) {
        // match the character to the monogram table entry
        for (int j = 0; j < monogram_table.size; j++) {
            // read the table entry at this index
            printf("%d %c\n", i, monogram_table.entries[j]->key[0]);


            //printf("%c\n", monogram_table->entries[j]->value);
            //if (uppercase_s[i] == monogram_table.entries[j]->key[0]) {
            //    score += monogram_table.entries[j]->value;
            //}
        }
    }

    free(uppercase_s);
    uppercase_s = NULL;

    return score;
}


// given a string, split the string into pairs of characters, and discard
// the last character if the string has an odd length
bigram_array split_into_bigrams(char* s) {
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

// free the memory allocated to a bigram array
void free_bigram_array(bigram_array* ba) {
    for (int i = 0; i < ba->num_bigrams; i++) {
        free(ba->bigrams[i]);
        ba->bigrams[i] = NULL;
    }
    free(ba->bigrams);
    ba->bigrams = NULL;
}


// given a string, convert the string to uppercase, and then split the string
// into bigrams, and calculate a score as a double, based on lookups into the
// bigram frequencies
double score_bigram(char* s) {
    double score = 0.0;
    char* uppercase_s = uppercase_string(s);
    bigram_array bigrams = split_into_bigrams(uppercase_s);

    for (int i = 0; i < bigrams.num_bigrams; i++) {
        // match the bigram to the bigram table entry
        for (int j = 0; j < bigram_table.size; j++) {
            if (strcmp(bigrams.bigrams[i], bigram_table.entries[j]->key) == 0) {
                score += bigram_table.entries[j]->value;
            }
        }
    }

    free(uppercase_s);
    return score;
}