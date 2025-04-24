#include <laye/diag.h>

#define KDSRC(Source, Location) \
    ((k_diag_source){ .name = (Source)->name, .text = (Source)->text, .use_byte_offset = true, .byte_offset = (Location) })

CHOIR_API void ly_err_invalid_character(k_diag* diag, ch_source* source, isize_t location) {
    k_diag_emitsf(diag, K_DIAG_ERROR, KDSRC(source, location), "Invalid character in source text.");
}

CHOIR_API void ly_err_unclosed_comment(k_diag* diag, ch_source* source, isize_t location) {
    k_diag_emitsf(diag, K_DIAG_ERROR, KDSRC(source, location), "Unclosed delimited comment.");
}
