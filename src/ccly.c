#include <stdio.h>

#include <choir/core.h>

int main(int argc, char** argv) {
    for (int i = 0; i < CH_TOKEN_KIND_COUNT; i++) {
        const char* name_spelling = ch_token_kind_name_get(cast(ch_token_kind) i);
        fprintf(stderr, "%s\n", name_spelling);
    }

    return 0;
}
