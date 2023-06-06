// read two equal length strings and produce their XOR combination
// input: 1c0111001f010100061a024b53535009181c
// input: 686974207468652062756c6c277320657965
// output: 746865206b696420646f6e277420706c6179
//
// move the utility functions and data structures into a separate file 

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "utils.h"

#define MAX_INPUT_HEX_LEN 256

void main(int argc, char * argv[]) {

    if (argc != 3) {
        printf("Usage: %s <hex string> <hex string>\n", argv[0]);
        return;
    }

    if (strlen(argv[1]) > MAX_INPUT_HEX_LEN) {
        printf("Input string too long\n");
        return;
    }

    if (strlen(argv[2]) > MAX_INPUT_HEX_LEN) {
        printf("Input string too long\n");
        return;
    }

    if (strlen(argv[1]) != strlen(argv[2])) {
        printf("Input strings must be the same length\n");
        return;
    }

    if (!is_valid_hex_string(argv[1])) {
        printf("Invalid hex string: %s\n", argv[1]);
        return;
    }

    if (!is_valid_hex_string(argv[2])) {
        printf("Invalid hex string: %s\n", argv[2]);
        return;
    }

    struct byte_array *input1 = hex_str_to_byte_array(argv[1]);
    struct byte_array *input2 = hex_str_to_byte_array(argv[2]);
    struct byte_array *output = xor_byte_arrays(input1, input2);

    
    char *output_hex = byte_array_to_hex_str(output);

    printf("%s\n", output_hex);
    
    free_byte_array(input1);
    free_byte_array(input2);
    free_byte_array(output);
    free(output_hex);
}

