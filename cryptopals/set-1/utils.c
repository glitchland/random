#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

// this function takes a character string, and calculates an integer score
// based on the frequency of characters in the string
// the higher the score, the more likely the string is to be english
// this is a very simple metric, and can be improved upon


// take a byte array and return a string of hex characters
char * byte_array_to_hex_str(struct byte_array *ba) {
    char *hex = calloc((ba->size * 2) + 1, sizeof(char));

    if (hex == NULL) {
        printf("Error allocating memory for hex string\n");
        exit(1);
    }

    for (int i = 0; i < ba->size; i++) {
        sprintf(hex + (i * 2), "%02x", ba->bytes[i]);
    }

    return hex;
}

// take two byte arrays as arguments, check if they are the same length
// then XOR them together and return the result as a byte array
struct byte_array * xor_byte_arrays(struct byte_array *ba1, struct byte_array *ba2) {
    struct byte_array *result = new_byte_array(ba1->size);

    if (ba1->size != ba2->size) {
        printf("Error: byte arrays must be the same length\n");
        exit(1);
    }

    for (int i = 0; i < ba1->size; i++) {
        result->bytes[i] = ba1->bytes[i] ^ ba2->bytes[i];
    }

    return result;
}

// check if a string of characters is a valid hex string
bool is_valid_hex_string(char *hex_str) {
    int i;

    for (i = 0; i < strlen(hex_str); i++) {
        if (!is_hex_char(hex_str[i])) {
            return false;
        }
    }

    return true;
}

bool is_hex_char(char c) {
    return (
        (c >= '0' && c <= '9') ||
        (c >= 'a' && c <= 'f') ||
        (c >= 'A' && c <= 'F')
    );
}

// allocate a new byte array with the given size
struct byte_array * new_byte_array(size_t size) {

    // allocate the struct and the bytes
    struct byte_array *array = malloc(
        sizeof(struct byte_array) + 
        (size * sizeof(unsigned char)) + 1
    );
    
    if (array == NULL) {
        printf("Error allocating memory for byte array\n");
        exit(1);
    }

    memset(array->bytes, 0, size + 1);
    array->size = size;

    return array;
}

// free a byte array struct, and nullify the pointer
void free_byte_array(struct byte_array *array) {
    free(array);
    array = NULL;
}

// allocate a new byte groups struct with the given size
struct byte_groups * new_byte_groups(size_t size) {
    struct byte_groups *groups = malloc(
        sizeof(struct byte_groups) + 
        (size * sizeof(unsigned char*)) + 1
    );
    
    if (groups == NULL) {
        printf("Error allocating memory for byte groups\n");
        exit(1);
    }
    
    memset(groups->groups, 0, size + 1);
    groups->size = size;

    return groups;
}

// free a byte groups struct, and nullify the pointers
void free_byte_groups(struct byte_groups *groups) {
    int i;

    for (i = 0; i < groups->size; i++) {
        free(groups->groups[i]);
        groups->groups[i] = NULL;
    }

    free(groups);
    groups = NULL;
}

// pad the string up to evenly divisible by 2 with 0s
struct byte_array * hex_str_to_byte_array(char *hex) {
    char *byte;
    int size = strlen(hex) / 2;
    struct byte_array *array = new_byte_array(size);

    for (int i = 0; i < size; i++) {
        byte = calloc(3, sizeof(char));

        for (int j = 0; j < 2; j++) {
            byte[j] = hex[(i * 2) + j];
        }

        array->bytes[i] = strtol(byte, NULL, 16);

        free(byte);
        byte = NULL;
    }

    return array;
}

// take a byte_array as an argument and return a string
char * byte_array_to_str(struct byte_array *bytes) {
    char *str = calloc(bytes->size + 1, sizeof(char));

    if (str == NULL) {
        printf("Error allocating memory for string\n");
        exit(1);
    }

    for (int i = 0; i < bytes->size; i++) {
        str[i] = bytes->bytes[i];
    }

    return str;
}
