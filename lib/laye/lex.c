#include <laye/core.h>

CHOIR_API void ly_lexer_init(ly_lexer* lexer, ch_context* context, ch_source* source) {
    if (lexer == nullptr) return;

    *lexer = (ly_lexer){
        .context = context,
        .source = source,
    };

    k_discard ly_lexer_peek(lexer, 0, &lexer->current_character, &lexer->current_stride);
}

static bool ly_lexer_peek(ly_lexer* lexer, int ahead, isize_t* out_character, isize_t* out_stride) {
    k_assert(lexer->context->diag, ahead >= 0, "Lexer does not support peeking backwards.");

    isize_t peek_position = ahead + lexer->current_position;
    if (peek_position >= lexer->source->text.count) {
        return false;
    }
}

CHOIR_API void ly_lexer_next_character(ly_lexer* lexer) {
}
