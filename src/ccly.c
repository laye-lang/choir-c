#include <choir/core.h>
#include <laye/core.h>

int main(int argc, char** argv) {
    k_arena string_arena = {0};
    k_arena_init(&string_arena);

    k_diag diag = {0};
    k_diag_formatted_state diag_userdata = {
        .output_stream = stderr,
    };
    k_diag_init(&diag, k_diag_formatted, &diag_userdata);

    ch_context context = {0};
    ch_context_init(&context, &diag, &string_arena);

    ch_source source = {
        .name = K_SV_CONST("foo.c"),
        .text = K_SV_CONST("#define FOO\\\r\n    0\nint main() {\r\n    return FOO;\n\r}")
    };

    ly_lexer lexer = {0};
    ly_lexer_init(&lexer, &context, &source);

    while (lexer.current_codepoint != 0) {
        fprintf(stderr, "%c", k_cast(int)lexer.current_codepoint);
        ly_lexer_next_character(&lexer);
    }

    return 0;
}
