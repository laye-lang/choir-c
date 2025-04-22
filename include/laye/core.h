#ifndef LAYE_CORE_H_
#define LAYE_CORE_H_

#if defined(__cplusplus)
extern "C" {
#endif // defined(__cplusplus)

#include <choir/core.h>
#include <kos/kos.h>

///===--------------------------------------===///
/// Data types.
///===--------------------------------------===///

/// @brief Describes under what circumstances a given keyword is available.
typedef enum ly_token_key {
    LY_TKKEY_NOT_KW = 0,
    /// @brief Indicates a keyword is available in all variants of C.
    LY_TKKEY_C = 1 << 0,
    /// @brief Indicates a keyword is available in all variants of Laye.
    LY_TKKEY_LAYE = 1 << 1,
    /// @brief Indicates a keyword is introduced to C in C99.
    LY_TKKEY_C99 = 1 << 2,
    /// @brief Indicates a keyword is introduced to C in C23.
    LY_TKKEY_C23 = 1 << 3,
    /// @brief Indicates a keyword is available if GNU extensions are enabled.
    LY_TKKEY_GNU = 1 << 4,
    /// @brief Indicates a keyword is available if Microsoft extensions are enabled.
    LY_TKKEY_MS = 1 << 5,
    /// @brief Indicates a keyword is available if Microsoft compatibility mode is enabled.
    LY_TKKEY_MS_COMPAT = 1 << 6,
    /// @brief Indicates a keyword is available if Clang extensions are enabled.
    LY_TKKEY_CLANG = 1 << 7,
    /// @brief Indicates a keyword is available if Choir extensions are enabled.
    LY_TKKEY_CHOIR = 1 << 8,
    /// @brief Indicates a keyword is available if 'bool' is a built-in type.
    LY_TKKEY_BOOL = 1 << 9,

    /// @brief The maximum key flag value.
    LY_TKKEY_MAX = LY_TKKEY_BOOL,
    /// @brief Indicates a keyword is available in all variants of C and Laye.
    LY_TKKEY_ALL = (LY_TKKEY_MAX) | (LY_TKKEY_MAX - 1),
} ly_token_key;

/// @brief Describes the kind of a source text token.
/// @ref ly_token
typedef enum ly_token_kind {
#define LY_TOKEN(id) LY_TK_##id,
#include "tokens.h"
    LY_TOKEN_KIND_COUNT,
} ly_token_kind;

/// @brief Token information for all variants of C and Laye.
/// @ref ly_token_kind
typedef struct ly_token ly_token;

/// @brief A dynamic array of tokens.
typedef struct ly_tokens {
    K_DA_DECLARE_INLINE(ly_token);
} ly_tokens;

typedef struct ly_translation_unit ly_translation_unit;

typedef struct ly_lexer ly_lexer;

typedef struct ly_preprocessor ly_preprocessor;

typedef struct ly_parser ly_parser;

typedef struct ly_sema ly_sema;

struct ly_token {
    /// @brief The kind of this token.
    /// Also used as a tag to select active data from the contained union, if applicable.
    ly_token_kind kind;

    /// @brief True if this token is the first on its line, preceded only by whitespace or delimited comments.
    bool at_start_of_line : 1;
    /// @brief True if this token has any whitespace before it, including line breaks and comments.
    bool has_whitespace_before : 1;
    /// @brief True if this token should not be considered for macro expansion.
    bool expansion_disabled : 1;

    /// @brief The source range of this token.
    ch_range range;

    union {
        /// @brief The textual value of this token for identifiers, preprocessing numbers and keywords.
        k_string_view text_value;
        /// @brief The value of this character constant.
        int32_t character_constant;
        /// @brief The value of this integer constant.
        int64_t integer_constant;
        /// @brief The value of this floating constant.
        double floating_constant;
        /// @brief The value of this string literal.
        k_string_view string_literal;
    };
};

///===--------------------------------------===///
/// Token API.
///===--------------------------------------===///

/// @brief Returns the name of the enum constant associated with this token kind.
/// @ref ly_token_kind
CHOIR_API const char* ly_token_kind_get_name(ly_token_kind kind);

/// @brief Returns the spelling of this token kind if it is represented by a single spelling or a canonical spelling from among aliases.
/// Otherwise returns `NULL`.
/// @ref ly_token_kind
CHOIR_API const char* ly_token_kind_get_spelling(ly_token_kind kind);

/// @brief Returns the keyword availability flags for this token kind if it's a keyword.
/// Otherwise returns `LY_TKKEY_NOT_KW`.
/// @ref ly_token_kind
/// @ref ly_token_key
CHOIR_API ly_token_key ly_token_kind_get_key(ly_token_kind kind);

///===--------------------------------------===///
/// Lexer API.
///===--------------------------------------===///

///===--------------------------------------===///
/// Preprocessor API.
///===--------------------------------------===///

///===--------------------------------------===///
/// Parser API.
///===--------------------------------------===///

///===--------------------------------------===///
/// Sema API.
///===--------------------------------------===///

#if defined(__cplusplus)
}
#endif // defined(__cplusplus)

#endif // LAYE_CORE_H_
