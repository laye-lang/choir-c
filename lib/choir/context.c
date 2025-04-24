#include <choir/core.h>

CHOIR_API void ch_context_init(ch_context* context, k_diag* diag, k_arena* string_arena) {
    *context = (ch_context){
        .diag = diag,
        .string_arena = string_arena,
    };
}
