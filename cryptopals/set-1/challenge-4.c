/*
  One of the 60-characters strings in puzzle-files/set-1/c4.txt has been encryted
  by single-character XOR. The goal is to find the string, decrypt it, and print
  the result.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xor.h"

#define RESOURCE_FILE "../puzzle-files/set-1/c4.txt"

struct file_lines {
    int count;
    char** lines;
};

struct file_lines* read_file_lines(FILE* file) {
    struct file_lines* result = malloc(sizeof(struct file_lines));
    result->count = 0;
    result->lines = NULL;

    char* line = NULL;
    size_t len = 0;
    size_t read;

    while ((read = getline(&line, &len, file)) != -1) {
        result->count++;
        result->lines = realloc(result->lines, result->count * sizeof(char*));
        line[strcspn(line, "\n")] = '\0';  // Strip newline character
        result->lines[result->count - 1] = line;
        line = NULL;
    }

    free(line);
    return result;
}

void free_file_lines(struct file_lines* lines) {
    for (int i = 0; i < lines->count; i++) {
        free(lines->lines[i]);
        lines->lines[i] = NULL;
    }
    free(lines->lines);
    lines->lines = NULL;
    free(lines);
    lines = NULL;
}

void print_file_lines(struct file_lines* lines) {
    for (int i = 0; i < lines->count; i++) {
        printf("%s", lines->lines[i]);
    }
    printf("\n");
}

void main() {
    // open a file for reading
    FILE* file = fopen(RESOURCE_FILE, "r");
    if (file == NULL) {
        printf("Error opening file\n");
        exit(1);
    }

    // read the file into a file_lines struct
    struct file_lines* data = read_file_lines(file);
    fclose(file);

    // for each line in the file
    struct key_score_table* best_scores = new_key_score_table(data->count);

    // brute force the key, and select the best candidate
    for (int i = 0; i < data->count; i++) {
        // remove the empty allocated key_score structs, as they are not needed
        free(best_scores->entries[i]);

        // allocate a new key_score struct for this line
        struct key_score* ks = select_best_xor_key_score(data->lines[i]);
        best_scores->entries[i] = ks;
    }

    // now that we have the best scores for each line, find the best line out
    // of all of them
    struct key_score* best_line = select_best_bigram_score(best_scores);

    print_key_score(best_line);

    free_score_key_table(best_scores);
    free_key_score(best_line);
    free_file_lines(data);
}