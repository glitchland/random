#include <stdio.h>

#include "utils.h"

static const char b64_table[64] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 
    'w', 'x', 'y', 'z', '0', '1', '2', '3', 
    '4', '5', '6', '7', '8', '9', '+', '/'
};

char base64_lookup(unsigned char v) {
    return b64_table[v];
}

// make groups from byte array
struct byte_groups * group_bytes(struct byte_array *input) {
    int numGroups = input->size / 3;

    struct byte_groups *groups = new_byte_groups(numGroups);

    for (int i = 0; i < numGroups; i++) {
        unsigned char *group = calloc(3, sizeof(unsigned char));

        for (int j = 0; j < 3; j++) {
            group[j] = input->bytes[(i * 3) + j];
        }

        groups->groups[i] = group;
    }

    return groups;
}


// given a byte array, pretty print the bytes
void debug_print_bytes(struct byte_array * array) {
    int len = array->size;

    printf("Length: %d\n", len);
    for (int i = 0; i < len; i++) {
        printf("%02x ", array->bytes[i]);
    }

    printf("\n");
}

// take a byte_groups struct and pretty print all of the groups
void debug_print_groups(struct byte_groups *groups) {
    for (int i = 0; i < groups->size; i++) {
        printf("Group %d: ", i);

        for (int j = 0; j < 3; j++) {
            printf("%02x ", groups->groups[i][j]);
        }

        printf("\n");
    }
}

// caller must free the returned byte array
struct byte_array * groups_to_b64_indexes(struct byte_groups *groups) {
    int numGroups = groups->size;
    struct byte_array *array = new_byte_array(numGroups * 4);

    for (int i = 0; i < numGroups; i++) {
        unsigned char *group = groups->groups[i];

        // convert the group of 3 bytes into a single 24 bit number
        unsigned int num = 0;
        for (int j = 0; j < 3; j++) {
            num = num << 8;
            num = num | group[j];
        }

        // convert the 24 bit number into 4 8 bit numbers with 6 bits active
        unsigned char nums[4];
        for (int j = 0; j < 4; j++) {
            nums[j] = num >> (18 - (j * 6));
            nums[j] = nums[j] & 0b00111111;
        }

        // convert the 8 bit numbers to decimal
        for (int j = 0; j < 4; j++) {
            array->bytes[(i * 4) + j] = nums[j];
        }
    }

    return array;
}

// caller must free the returned byte array
struct byte_array * indexes_to_base64_array(struct byte_array *indexes) {
    struct byte_array *b64_chars = new_byte_array(indexes->size);

    for (int i = 0; i < indexes->size; i++) {
        b64_chars->bytes[i] = base64_lookup(indexes->bytes[i]);
    }

    return b64_chars;
}

// take a byte_array as an argument and return a string 
char * base64_array_to_str(struct byte_array *bytes) {
    return byte_array_to_str(bytes);
}

// https://base64.guru/learn/base64-algorithm/encode
char * hex_to_base64(struct byte_array *bytes) {
    struct byte_groups *groups = group_bytes(bytes);
    struct byte_array *b64_indexes = groups_to_b64_indexes(groups);
    struct byte_array *b64_chars = indexes_to_base64_array(b64_indexes);
    char *b64_str = base64_array_to_str(b64_chars);

    free_byte_groups(groups);
    free_byte_array(b64_indexes);
    free_byte_array(b64_chars);

    return b64_str;
}