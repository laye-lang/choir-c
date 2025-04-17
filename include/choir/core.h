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

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef float float32;
typedef double float64;

typedef size_t usize;
typedef ptrdiff_t isize;

/// @brief Describes under what circumstances a given keyword is available.
typedef enum ch_token_key {
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
    /// @brief Indicates a keyword is available if Borland extensions are enabled.
    CH_TKKEY_BORLAND = 1 << 8,
    /// @brief Indicates a keyword is available if Choir extensions are enabled.
    CH_TKKEY_CHOIR = 1 << 9,
    /// @brief Indicates a keyword is available if 'bool' is a built-in type.
    CH_TKKEY_BOOL = 1 << 10,

    /// @brief The maximum key flag value.
    CH_TKKEY_MAX = CH_TKKEY_BOOL,
    /// @brief Indicates a keyword is available in all variants of C and Laye.
    CH_TKKEY_ALL = (CH_TKKEY_MAX) | (CH_TKKEY_MAX - 1),
} ch_token_key;

/// @brief Describes the kind of a source text token.
/// @ref ch_token
typedef enum ch_token_kind {
#define CH_TOKEN(id) CH_TK_##id,
#include "tokens.inc"
    CH_TOKEN_KIND_COUNT,
} ch_token_kind;

/// @brief Returns the name of the enum constant associated with this token kind.
/// @ref ch_token_kind
CHOIR_API const char* ch_token_kind_name_get(ch_token_kind kind);

/// @brief Returns the spelling of this token kind if it is represented by a single spelling or a canonical spelling from among aliases.
/// Otherwise returns NULL.
/// @ref ch_token_kind
CHOIR_API const char* ch_token_kind_spelling_get(ch_token_kind kind);

#if defined(__cplusplus)
}
#endif // defined(__cplusplus)

#endif // CHOIR_CORE_H_
