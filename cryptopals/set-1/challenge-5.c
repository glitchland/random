// Implement repeating key mode XOR encryption with arbitrary key length
// and plaintext length.
#include <string.h>

#include "utils.h"
#include "xor.h"

void main() {
    char *clear_text = "Burning 'em, if you ain't quick and nimble\n"
                       "I go crazy when I hear a cymbal";
    char *key = "ICE";

    struct byte_array* clear_text_bytes = byte_array_from_str(clear_text);
    struct byte_array* key_bytes = byte_array_from_str(key);

    // this function modifies the clear_text_bytes array in place
    xor_with_repeating_key(clear_text_bytes, key_bytes);

    char *expected = "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d62"
                     "3d63343c2a26226324272765272a282b2f20430a652e2c65"
                     "2a3124333a653e2b2027630c692b20283165286326302e27"
                     "282f";
    
    char *actual = byte_array_to_hex_str(clear_text_bytes);

    if (strcmp(expected, actual) == 0) {
        printf("Success!\n");
    } else {
        printf("Failure!\n");
        printf("Expected: %s\n", expected);
        printf("Actual: %s\n", actual);
    }

    free(actual);
    actual = NULL;
    free_byte_array(clear_text_bytes);
    free_byte_array(key_bytes);
}