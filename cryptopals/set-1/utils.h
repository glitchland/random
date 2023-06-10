#pragma once 

#include <stdlib.h>
#include <stdbool.h>

struct byte_groups {
    size_t size;
    unsigned char* groups[];
};

struct byte_array {
    size_t size;
    unsigned char bytes[];
};

struct byte_array * new_byte_array(size_t size);
struct byte_groups * new_byte_groups(size_t size);
void free_byte_array(struct byte_array * ba);
void free_byte_groups(struct byte_groups * bg);

struct byte_array * hex_str_to_byte_array(char *hex);

char * byte_array_to_str(struct byte_array *bytes);
struct byte_array * xor_byte_arrays(struct byte_array *ba1, struct byte_array *ba2);
char * byte_array_to_hex_str(struct byte_array *ba);
struct byte_array* str_to_byte_array(char *str);

bool is_valid_hex_string(char *hex_str);
bool is_hex_char(char c);
