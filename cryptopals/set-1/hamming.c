#include "utils.h"

// given two byte arrays, return the hamming distance between them
int hamming_distance(struct byte_array *a, struct byte_array *b) {
    int distance = 0;

    for (int i = 0; i < a->size; i++) {
        unsigned char x = a->bytes[i];
        unsigned char y = b->bytes[i];

        // xor the two bytes together
        unsigned char z = x ^ y;

        // count the number of 1 bits in the result
        while (z) {
            distance++;
            z &= z - 1;
        }
    }

    return distance;
}