#include <laye/core.h>

static bool ly_lexer_peek_raw(ly_lexer* lexer, isize_t peek_position, int32_t* out_codepoint, isize_t* out_stride);
static int32_t ly_lexer_peek(ly_lexer* lexer, int ahead);

CHOIR_API void ly_lexer_init(ly_lexer* lexer, ch_context* context, ch_source* source) {
    if (lexer == nullptr) return;

    *lexer = (ly_lexer){
        .context = context,
        .source = source,
        // Initialize tracking for __FILE__ and __LINE__.
        .current_file_name = source->name,
        .current_line_number = 1,
    };

    if (!ly_lexer_peek_raw(lexer, 0, &lexer->current_codepoint, &lexer->current_stride)) {
        lexer->current_codepoint = 0;
        lexer->current_stride = 0;
    }
}

static bool ly_lexer_peek_raw(ly_lexer* lexer, isize_t peek_position, int32_t* out_codepoint, isize_t* out_stride) {
    assert(lexer != nullptr);

    // TODO(local): Consider if supporting other encodings is worth doing.
    // If it is, we'll have a custom character decoder replace this hard-coded call to our UTF-8 decoder.

    const char* text_data = lexer->source->text.data;
    isize_t text_count = lexer->source->text.count;

    int32_t codepoint = 0;
    isize_t stride = 0;

    k_unicode_decode_result decode_result = k_utf8_decode(text_data, text_count, peek_position, &codepoint, &stride);
    if (decode_result != K_UNICODE_SUCCESS) {
        return false;
    }

    peek_position += stride;

    if (peek_position < text_count) {
        int32_t ahead_codepoint = 0;
        isize_t ahead_stride = 0;

        // The sequence '\n\r' should be handled as a single '\n'.
        if (codepoint == '\n' && K_UNICODE_SUCCESS == k_utf8_decode(text_data, text_count, peek_position, &ahead_codepoint, &ahead_stride) && ahead_codepoint == '\r') {
            stride += ahead_stride;
            peek_position += ahead_stride;
        }

        // The character '\n' and sequence '\r\n' should be handled as a single '\n'.
        if (codepoint == '\r') {
            codepoint = '\n';
            if (K_UNICODE_SUCCESS == k_utf8_decode(text_data, text_count, peek_position, &ahead_codepoint, &ahead_stride) && ahead_codepoint == '\n') {
                stride += ahead_stride;
                peek_position += ahead_stride;
            }
        }

        // TODO(local): since escaping a newline turns it into a space at this level, is this something the preprocessor thinks increases the current line number?
        // I don't think it does, but we need to test it and check the standard.

        // The sequences '\\\n', '\\\r', '\\\n\r' and '\\\r\n' should all be handled as a single ' '.
        if (codepoint == '\\' && K_UNICODE_SUCCESS == k_utf8_decode(text_data, text_count, peek_position, &ahead_codepoint, &ahead_stride) && (ahead_codepoint == '\r' || ahead_codepoint == '\n')) {
            codepoint = ' ';
            stride += ahead_stride;
            peek_position += ahead_stride;

            int32_t other_newline_char = ahead_codepoint == '\r' ? '\n' : '\r';
            if (K_UNICODE_SUCCESS == k_utf8_decode(text_data, text_count, peek_position, &ahead_codepoint, &ahead_stride) && ahead_codepoint == other_newline_char) {
                stride += ahead_stride;
                peek_position += ahead_stride;
            }
        }
    }

    if (out_codepoint != nullptr) *out_codepoint = codepoint;
    if (out_stride != nullptr) *out_stride = stride;

    return true;
}

static int32_t ly_lexer_peek(ly_lexer* lexer, int ahead) {
    assert(lexer != nullptr);
    k_assert(lexer->context->diag, ahead >= 0, "Lexer does not support peeking backwards.");

    // we only have to do on-the-fly decoding for ahead > 0.
    if (ahead == 0) {
        return lexer->current_codepoint;
    }

    int32_t codepoint = 0;
    isize_t stride = 0;

    isize_t peek_position = lexer->current_position + lexer->current_stride;
    for (int i = 0; i < ahead; i++) {
        if (!ly_lexer_peek_raw(lexer, peek_position, &codepoint, &stride)) {
            return 0;
        }

        peek_position += stride;
    }

    return codepoint;
}

CHOIR_API void ly_lexer_next_character(ly_lexer* lexer) {
    assert(lexer != nullptr);

    // there's nothing to do if the last character had 0 stride.
    if (lexer->current_stride == 0) {
        lexer->current_codepoint = 0;
        return;
    }

    lexer->current_position += lexer->current_stride;
    if (!ly_lexer_peek_raw(lexer, lexer->current_position, &lexer->current_codepoint, &lexer->current_stride)) {
        lexer->current_codepoint = 0;
        lexer->current_stride = 0;
    }
}
