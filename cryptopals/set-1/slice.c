#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "slice.h"

slice new_slice(size_t initial_size, size_t element_size) {
    slice s;
    s.array = calloc(initial_size, element_size);
    s.used = 0;
    s.size = initial_size;
    s.element_size = element_size;
    return s;
}

void slice_append(slice *s, void *element) {
    if (s->used == s->size) {
        s->size *= 2;
        s->array = realloc(s->array, s->size * s->element_size);
    }
    memcpy((char *)s->array + s->used * s->element_size, element, s->element_size);
    s->used++;
}

void free_slice(slice *s) {
    free(s->array);
    s->array = NULL;
    s->used = s->size = 0;
    s->element_size = 0;
}

void* get_element_at(slice *s, size_t index) {
    if (index >= s->used) {
        return NULL;  // Out of bounds
    }
    return (char *)s->array + index * s->element_size;
}

void set_element_at(slice *s, size_t index, void* new_element) {
    if (index >= s->used) {
        fprintf(stderr, "Index out of bounds\n");
        exit(EXIT_FAILURE);
    }

    // Copy the new element to the appropriate position in the slice's array
    memcpy((char *)s->array + index * s->element_size, new_element, s->element_size);
}

size_t slice_len(slice* s) {
    return s->used;
}

//////////////////////////////////////////////
// byte slices
slice* new_byte_slice(size_t initialSize) {
    slice* byte_slice = calloc(1, sizeof(slice));
    if (byte_slice == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }
    
    *byte_slice = new_slice(initialSize, sizeof(unsigned char));
    return byte_slice;
}

void append_to_byte_slice(slice *s, unsigned char element) {
    slice_append(s, &element);
}

unsigned char get_byte_at(slice *s, size_t index) {
    unsigned char *element = (unsigned char *)get_element_at(s, index);
    if (element == NULL) {
        fprintf(stderr, "Index out of bounds\n");
        exit(EXIT_FAILURE);
    }
    return *element;
}

void set_byte_at(slice* s, size_t index, unsigned char new_element) {
    if (index >= s->used) {
        fprintf(stderr, "Index out of bounds\n");
        exit(EXIT_FAILURE);
    }

    unsigned char* array = (unsigned char*)s->array;
    array[index] = new_element;
}

slice* byte_slice_from_str(char *str) {
    size_t str_length = strlen(str);
    slice* byte_slice = calloc(1, sizeof(slice));
    if (byte_slice == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }
    
    byte_slice = new_byte_slice(str_length);
    for (size_t i = 0; i < str_length; i++) {
        append_to_byte_slice(byte_slice, (unsigned char)str[i]);
    }
    return byte_slice;
}

char* byte_slice_to_string(slice* s) {
    char* str = calloc(s->used + 1, sizeof(char));
    if (str == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < s->used; i++) {
        str[i] = (char)get_byte_at(s, i);
    }
    str[s->used] = '\0';  // null-terminate the string
    return str;
}

slice* read_file_to_byte_slice(char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    slice* byte_slice = calloc(1, sizeof(slice));
    if (byte_slice == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }

    byte_slice = new_byte_slice(1);
    int ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ch != '\n' && ch != '\r') {
            unsigned char uchar = (unsigned char)ch;
            append_to_byte_slice(byte_slice, uchar);
        }
    }

    fclose(file);
    return byte_slice;
}

slice* hex_string_to_byte_slice(const char* hex_str) {
    // Each byte in the output is represented by two characters in the input.
    size_t len = strlen(hex_str);
    if (len % 2 != 0) {
        fprintf(stderr, "Invalid hex string length\n");
        exit(EXIT_FAILURE);
    }

    slice* byte_slice = calloc(1, sizeof(slice));
    if (byte_slice == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }

    byte_slice = new_byte_slice(len / 2);
    for (size_t i = 0; i < len; i += 2) {
        unsigned int byte;
        if (sscanf(hex_str + i, "%2x", &byte) != 1) {
            fprintf(stderr, "Failed to parse hex string\n");
            exit(EXIT_FAILURE);
        }
        append_to_byte_slice(byte_slice, (unsigned char)byte);
    }
    return byte_slice;
}

char* byte_slice_to_hex_string(slice* s) {
    char* hex_str = calloc(s->used * 2 + 1, sizeof(char));  // each byte becomes 2 hex digits
    if (hex_str == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < s->used; i++) {
        unsigned char byte = get_byte_at(s, i);
        sprintf(hex_str + i * 2, "%02x", byte);
    }

    return hex_str;
}

slice* xor_byte_slices(slice* s1, slice* s2) {
    if (s1->used != s2->used) {
        fprintf(stderr, "Error: byte slices are not of the same length\n");
        exit(EXIT_FAILURE);
    }

    slice* result = calloc(1, sizeof(slice));
    if (result == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }

    result = new_byte_slice(s1->used);
    for (size_t i = 0; i < s1->used; i++) {
        unsigned char byte1 = get_byte_at(s1, i);
        unsigned char byte2 = get_byte_at(s2, i);
        append_to_byte_slice(result, byte1 ^ byte2);
    }

    return result;
}
