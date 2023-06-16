#pragma once

#include <stdlib.h>

typedef struct {
    void *array;
    size_t used;
    size_t size;
    size_t element_size;
} slice;

void free_slice(slice *s);
size_t slice_len(slice* s);
slice* new_byte_slice(size_t initialSize);
void append_to_byte_slice(slice *s, unsigned char element);
unsigned char get_byte_at(slice *s, size_t index);
void set_byte_at(slice* s, size_t index, unsigned char new_element);
slice* byte_slice_from_string(char *str);
char* byte_slice_to_string(slice* s);
slice* read_file_to_byte_slice(char* filename);
slice* hex_string_to_byte_slice(const char* hex_str);
char* byte_slice_to_hex_string(slice* s);
slice* xor_byte_slices(slice* s1, slice* s2);
void debug_print_byte_slice(slice* s);