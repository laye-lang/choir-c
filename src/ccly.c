#include <stdio.h>
/// include choir last
#include <choir/core.h>

int main(int argc, char** argv) {
    for (int i = 0; i < CH_TOKEN_KIND_COUNT; i++) {
        ch_token_kind kind = cast(ch_token_kind) i;

        const char* name_spelling = ch_token_kind_name_get(kind);
        fprintf(stderr, "%s", name_spelling);

        const char* token_spelling = ch_token_kind_spelling_get(kind);
        if (token_spelling != nullptr) {
            fprintf(stderr, "  ::  %s", token_spelling);
        }

        ch_token_key token_key = ch_token_kind_key_get(kind);
        if (token_key != CH_TKKEY_NOT_KW) {
            fprintf(stderr, "  ::  %d", token_key);
        }

        fprintf(stderr, "\n");
    }

    return 0;
}
