#include "slice.h"
#include "utils.h"

// given two byte arrays, return the hamming distance between them
int hamming_distance(slice *a, slice *b) {
    int distance = 0;

    for (int i = 0; i < a->size; i++) {
        unsigned char x = get_byte_at(a, i);
        unsigned char y = get_byte_at(b, i);

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

// return the normalized hamming distance between two byte arrays
double normalized_hamming_distance(slice *a, slice *b) {
    return (double) hamming_distance(a, b) / (double) a->size;
}