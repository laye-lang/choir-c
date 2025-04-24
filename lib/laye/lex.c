#include <laye/core.h>
#include <laye/diag.h>

static bool ly_lexer_peek_raw(ly_lexer* lexer, isize_t peek_position, int32_t* out_codepoint, isize_t* out_stride);
static int32_t ly_lexer_peek(ly_lexer* lexer, int ahead);

CHOIR_API void ly_lexer_init(ly_lexer* lexer, ch_context* context, ch_source* source, ly_lexer_mode mode) {
    if (lexer == nullptr) return;

    *lexer = (ly_lexer){
        .context = context,
        .source = source,
        .is_at_start_of_line = true,
        .mode = mode,
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
        if (0 != (lexer->mode & LY_LEXMODE_C) && codepoint == '\\' && K_UNICODE_SUCCESS == k_utf8_decode(text_data, text_count, peek_position, &ahead_codepoint, &ahead_stride) && (ahead_codepoint == '\r' || ahead_codepoint == '\n')) {
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

    if (lexer->current_codepoint == '\n') {
        lexer->current_line_number++;
        lexer->is_at_start_of_line = true;
    }

    lexer->current_position += lexer->current_stride;
    if (!ly_lexer_peek_raw(lexer, lexer->current_position, &lexer->current_codepoint, &lexer->current_stride)) {
        lexer->current_codepoint = 0;
        lexer->current_stride = 0;
    }
}

static void ly_lexer_read_relevant_trivia(ly_lexer* lexer, bool is_leading) {
    assert(lexer != nullptr);

    ly_lexer_mode mode = lexer->mode;
    isize_t begin_position = lexer->current_position;

    while (lexer->current_codepoint != 0) {
        int32_t c = lexer->current_codepoint;
        switch (c) {
            default: goto done_reading_trivia;

            case '#': {
                if (lexer->current_position == 0 && ly_lexer_peek(lexer, 1) == '!') {
                    ly_lexer_next_character(lexer); // omnom '#'
                    ly_lexer_next_character(lexer); // omnom '!'
                    // TODO(local): store the text of this shebang and throw it in a trivia, it'll be important eventually.
                    while (lexer->current_codepoint != 0 && lexer->current_codepoint != '\n') {
                        ly_lexer_next_character(lexer); // omnom anything that isn't the end of line/file
                    }
                } else goto done_reading_trivia;
            } break;

            case '/': {
                if (ly_lexer_peek(lexer, 1) == '/') {
                    ly_lexer_next_character(lexer); // omnom '/'
                    ly_lexer_next_character(lexer); // omnom '/'
                    // TODO(local): store the text of this line comment and throw it in a trivia, it'll be important eventually.
                    while (lexer->current_codepoint != 0 && lexer->current_codepoint != '\n') {
                        ly_lexer_next_character(lexer); // omnom anything that isn't the end of line/file
                    }
                } else if (ly_lexer_peek(lexer, 1) == '*') {
                    ly_lexer_next_character(lexer); // omnom '/'
                    ly_lexer_next_character(lexer); // omnom '*'
                    // TODO(local): store the text of this line comment and throw it in a trivia, it'll be important eventually.
                    int comment_nesting = 1;
                    int32_t prev_codepoint = 0;
                    while (lexer->current_codepoint != 0 && comment_nesting > 0) {
                        int32_t c = lexer->current_codepoint;
                        if (c == '/' && prev_codepoint == '*') {
                            prev_codepoint = 0;
                            comment_nesting--;
                        } else if (0 != (mode & LY_LEXMODE_LAYE) && c == '*' && prev_codepoint == '/') {
                            prev_codepoint = 0;
                            comment_nesting++;
                        }

                        prev_codepoint = c;
                        ly_lexer_next_character(lexer); // omnom anything until we get to the end of comment/file
                    }

                    if (comment_nesting > 0) {
                        ly_err_unclosed_comment(lexer->context->diag, lexer->source, begin_position);
                    }
                } else goto done_reading_trivia;
            } break;

            case ' ':
            case '\t':
            case '\n':
            case '\v': {
                ly_lexer_next_character(lexer); // omnom whitespace
            } break;
        }
    }

done_reading_trivia:;
    // TODO(local): When we're ready to *store* trivia, put it in a list and return it here.
    return;
}

CHOIR_API ly_token ly_lexer_read_pp_token(ly_lexer* lexer) {
    assert(lexer != nullptr);

    isize_t begin_position = lexer->current_position;
    // TODO(local): Store the trivia in a trivia list for later.
    ly_lexer_read_relevant_trivia(lexer, true);

    ly_lexer_mode mode = lexer->mode;
    bool at_start_of_line = lexer->is_at_start_of_line;
    bool has_white_space_before = begin_position != lexer->current_position;

    ly_token_kind kind = LY_TK_INVALID;
    int32_t c = lexer->current_codepoint;

    begin_position = lexer->current_position;
    switch (c) {
        default: {
            ly_lexer_next_character(lexer);
            ly_err_invalid_character(lexer->context->diag, lexer->source, begin_position);
        } break;
    }

    isize_t end_position = lexer->current_position;
    ch_asserts(lexer->context->diag, end_position > begin_position, lexer->source, begin_position, "Lexer did not consume a character.");

    ch_range range = {
        .source = lexer->source,
        .begin = begin_position,
        .end = end_position,
    };

    // TODO(local): Store the trivia in a trivia list for later.
    ly_lexer_read_relevant_trivia(lexer, false);

    return (ly_token){
        .kind = kind,
        .at_start_of_line = at_start_of_line,
        .has_white_space_before = has_white_space_before,
        .range = range,
    };
}

CHOIR_API void ly_lexer_push_mode(ly_lexer* lexer, ly_lexer_mode mode) {
}

CHOIR_API void ly_lexer_pop_mode(ly_lexer* lexer) {
}
