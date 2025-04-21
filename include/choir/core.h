#ifndef CHOIR_CORE_H_
#define CHOIR_CORE_H_

#if defined(__cplusplus)
extern "C" {
#endif // defined(__cplusplus)

#include "config.h"
#include "macros.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

///===--------------------------------------===///
/// Data types.
///===--------------------------------------===///

typedef ptrdiff_t isize_t;

/// @brief An immutable view into underlying, non-owned string data.
typedef struct ch_sting_view {
    const char* data;
    isize_t count;
} ch_string_view;

/// @brief A mutable, owning string builder.
typedef struct ch_string {
    CH_DA_DECLARE_INLINE(char);
} ch_string;

/// @brief Source text from any language or input source.
typedef struct ch_source {
    /// @brief The name of this source, usually a canonical file path for a source file or an angle-bracketted '<compiler-internal>' name.
    ch_string_view name;
    /// @brief The full text of this source.
    ch_string_view text;
    /// @brief True if this source represents a "system" file and should be treated more lax by the language semantics.
    /// Used primarily for system C headers, which may make liberal use of extensions or incompatible features.
    bool is_system_source : 1;
} ch_source;

/// @brief A 0-based byte location within the text of a source.
typedef isize_t ch_location;

/// @brief A 0-based byte range within the text of the referenced source.
/// The byte length of this range is given by 'end' - 'begin'.
typedef struct ch_range {
    /// @brief The source this range applies to.
    ch_source* source;
    /// @brief The beginning byte offset of this range.
    ch_location begin;
    /// @brief The ending byte offset of this range.
    ch_location end;
} ch_range;

/// @brief Describes under what circumstances a given keyword is available.
typedef enum ch_token_key {
    CH_TKKEY_NOT_KW = 0,
    /// @brief Indicates a keyword is available in all variants of C.
    CH_TKKEY_C = 1 << 0,
    /// @brief Indicates a keyword is available in all variants of Laye.
    CH_TKKEY_LAYE = 1 << 1,
    /// @brief Indicates a keyword is introduced to C in C99.
    CH_TKKEY_C99 = 1 << 2,
    /// @brief Indicates a keyword is introduced to C in C23.
    CH_TKKEY_C23 = 1 << 3,
    /// @brief Indicates a keyword is available if GNU extensions are enabled.
    CH_TKKEY_GNU = 1 << 4,
    /// @brief Indicates a keyword is available if Microsoft extensions are enabled.
    CH_TKKEY_MS = 1 << 5,
    /// @brief Indicates a keyword is available if Microsoft compatibility mode is enabled.
    CH_TKKEY_MS_COMPAT = 1 << 6,
    /// @brief Indicates a keyword is available if Clang extensions are enabled.
    CH_TKKEY_CLANG = 1 << 7,
    /// @brief Indicates a keyword is available if Choir extensions are enabled.
    CH_TKKEY_CHOIR = 1 << 8,
    /// @brief Indicates a keyword is available if 'bool' is a built-in type.
    CH_TKKEY_BOOL = 1 << 9,

    /// @brief The maximum key flag value.
    CH_TKKEY_MAX = CH_TKKEY_BOOL,
    /// @brief Indicates a keyword is available in all variants of C and Laye.
    CH_TKKEY_ALL = (CH_TKKEY_MAX) | (CH_TKKEY_MAX - 1),
} ch_token_key;

/// @brief Describes the kind of a source text token.
/// @ref ch_token
typedef enum ch_token_kind {
#define CH_TOKEN(id) CH_TK_##id,
#include "tokens.h"
    CH_TOKEN_KIND_COUNT,
} ch_token_kind;

/// @brief Token information for all variants of C and Laye.
/// @ref ch_token_kind
typedef struct ch_token {
    /// @brief The kind of this token.
    /// Also used as a tag to select active data from the contained union, if applicable.
    ch_token_kind kind;

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
        ch_string_view text_value;
        /// @brief The value of this character constant.
        int32_t character_constant;
        /// @brief The value of this integer constant.
        int64_t integer_constant;
        /// @brief The value of this floating constant.
        double floating_constant;
        /// @brief The value of this string literal.
        ch_string_view string_literal;
    };
} ch_token;

CH_DA_DECLARE(ch_tokens, ch_token);

typedef struct ch_lexer ch_lexer;
typedef struct ch_preprocessor ch_preprocessor;
typedef struct ch_parser ch_parser;
typedef struct ch_sema ch_sema;

///===--------------------------------------===///
/// Strings API.
///===--------------------------------------===///

///===--------------------------------------===///
/// Context API.
///===--------------------------------------===///

///===--------------------------------------===///
/// Token API.
///===--------------------------------------===///

/// @brief Returns the name of the enum constant associated with this token kind.
/// @ref ch_token_kind
CHOIR_API const char* ch_token_kind_get_name(ch_token_kind kind);

/// @brief Returns the spelling of this token kind if it is represented by a single spelling or a canonical spelling from among aliases.
/// Otherwise returns `NULL`.
/// @ref ch_token_kind
CHOIR_API const char* ch_token_kind_get_spelling(ch_token_kind kind);

/// @brief Returns the keyword availability flags for this token kind if it's a keyword.
/// Otherwise returns `CH_TKKEY_NOT_KW`.
/// @ref ch_token_kind
/// @ref ch_token_key
CHOIR_API ch_token_key ch_token_kind_get_key(ch_token_kind kind);

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

#endif // CHOIR_CORE_H_
