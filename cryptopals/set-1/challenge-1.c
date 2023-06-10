
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base64.h"
#include "utils.h"

// This is a solution for cryptopals set 1 challenge 1. Bade64 encoding.
void main () {
    char *hex = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
    char *answer = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";
    
    // convert the hex string to an array of unsigned bytes
    struct byte_array *bytes = hex_str_to_byte_array(hex);

    // base 64 encode the byte array
    char *b64 = hex_to_base64(bytes);

    // print the base 64 encoded string
    printf("Input:\t%s\n", hex);
    printf("Output:\t%s\n", b64);

    // check if the output matches the answer
    if(strcmp(b64, answer) == 0) {
        printf("Success!\n");
    } else {
        printf("Failure!\n");
    }

    // free the memory
    free_byte_array(bytes);
    free(b64);
    b64 = NULL;

    return;
}
