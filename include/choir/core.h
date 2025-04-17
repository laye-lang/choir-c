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

typedef enum ch_token_key {
    CH_TKKEY_C = 1 << 0,
    CH_TKKEY_LAYE = 1 << 1,
    CH_TKKEY_C99 = 1 << 2,
    CH_TKKEY_C23 = 1 << 3,
    CH_TKKEY_GNU = 1 << 4,
    CH_TKKEY_MS = 1 << 5,
    CH_TKKEY_MS_COMPAT = 1 << 6,
    CH_TKKEY_CLANG = 1 << 7,
    CH_TKKEY_BORLAND = 1 << 8,
    CH_TKKEY_CHOIR = 1 << 9,
    CH_TKKEY_BOOL = 1 << 10,
    CH_TKKEY_WCHAR = 1 << 11,

    CH_TKKEY_MAX = CH_TKKEY_LAYE,
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

#if defined(__cplusplus)
}
#endif // defined(__cplusplus)

#endif // CHOIR_CORE_H_
