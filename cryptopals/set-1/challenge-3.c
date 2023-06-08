#include <stdio.h>
#include <stdbool.h>

#include "frequency.h"
#include "utils.h"
#include "xor.h"

void main() {
    char *hex_str = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";

    struct key_score* top_score = select_best_xor_key_score(hex_str);

    print_key_score(top_score);

    free_key_score(top_score);
}