#include <stdio.h>
/// include choir last
#include <choir/core.h>
#include <laye/core.h>

int main(int argc, char** argv) {
    for (isize_t i = 0; i < LY_TOKEN_KIND_COUNT; i++) {
        ly_token_kind kind = k_cast(ly_token_kind) i;

        const char* name_spelling = ly_token_kind_get_name(kind);
        fprintf(stderr, "%s", name_spelling);

        const char* token_spelling = ly_token_kind_get_spelling(kind);
        if (token_spelling != nullptr) {
            fprintf(stderr, "  ::  %s", token_spelling);
        }

        ly_token_key token_key = ly_token_kind_get_key(kind);
        if (token_key != LY_TKKEY_NOT_KW) {
            fprintf(stderr, "  ::  %d", token_key);
        }

        fprintf(stderr, "\n");
    }

    return 0;
}
