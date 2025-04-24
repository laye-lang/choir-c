#include <kos/kos.h>

k_unicode_decode_result k_utf8_decode(const char* data, isize_t data_count, isize_t offset, int32_t* out_codepoint, isize_t* out_stride) {
    if (data == nullptr || data_count <= 0) {
        return K_UNICODE_END_OF_DATA;
    }

    if (offset < 0 || offset >= data_count) {
        return K_UNICODE_OUT_OF_RANGE;
    }

    char c = data[offset];

    // 0yyyzzzz is a single byte
    if ((c & 0x80) == 0) {
        if (out_codepoint != nullptr) *out_codepoint = c;
        if (out_stride != nullptr) *out_stride = 1;
        return K_UNICODE_SUCCESS;
    }

    // 110xxxyy 10yyzzzz is two bytes
    if ((c & 0xE0) == 0xC0) {
        if (data_count < 2) return K_UNICODE_END_OF_DATA;

        char c1 = data[offset + 1];
        if ((c1 & 0xC0) != 0x80) return K_UNICODE_INVALID_CONTINUE_BYTE;

        if (out_codepoint != nullptr) *out_codepoint = ((c & 0x1F) << 6) | (c1 & 0x3F);
        if (out_stride != nullptr) *out_stride = 2;
        return K_UNICODE_SUCCESS;
    }

    // 1110wwww 10xxxxyy 10yyzzzz is three bytes
    if ((c & 0xF0) == 0xE0) {
        if (data_count < 3) return K_UNICODE_END_OF_DATA;

        char c1 = data[offset + 1];
        if ((c1 & 0xC0) != 0x80) return K_UNICODE_INVALID_CONTINUE_BYTE;

        char c2 = data[offset + 2];
        if ((c2 & 0xC0) != 0x80) return K_UNICODE_INVALID_CONTINUE_BYTE;

        if (out_codepoint != nullptr) *out_codepoint = ((c & 0x0F) << 12) | ((c1 & 0x3F) << 6) | (c2 & 0x3F);
        if (out_stride != nullptr) *out_stride = 3;
        return K_UNICODE_SUCCESS;
    }

    // 11110uvv 10vvwwww 10xxxxyy 10yyzzzz is four bytes
    if ((c & 0xF8) == 0xF0) {
        if (data_count < 4) return K_UNICODE_END_OF_DATA;

        char c1 = data[offset + 1];
        if ((c1 & 0xC0) != 0x80) return K_UNICODE_INVALID_CONTINUE_BYTE;

        char c2 = data[offset + 2];
        if ((c2 & 0xC0) != 0x80) return K_UNICODE_INVALID_CONTINUE_BYTE;

        char c3 = data[offset + 3];
        if ((c3 & 0xC0) != 0x80) return K_UNICODE_INVALID_CONTINUE_BYTE;

        if (out_codepoint != nullptr) *out_codepoint = ((c & 0x07) << 18) | ((c1 & 0x3F) << 12) | ((c2 & 0x3F) << 6) | (c3 & 0x3F);
        if (out_stride != nullptr) *out_stride = 4;
        return K_UNICODE_SUCCESS;
    }

    return K_UNICODE_INVALID_START_BYTE;
}
