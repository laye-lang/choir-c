#ifndef CHOIR_CORE_H_
#define CHOIR_CORE_H_

#if defined(__cplusplus)
extern "C" {
#endif // defined(__cplusplus)

#include <kos/kos.h>

#if defined(CHOIR_BUILD_DLL)
#    if defined(CHOIR_LIB)
#        define CHOIR_API __declspec(dllexport)
#    else
#        define CHOIR_API __declspec(dllimport)
#    endif
#else // !CHOIR_BUILD_DLL
#    define CHOIR_API extern
#endif // CHOIR_BUILD_DLL

#define ch_asserts(Diag, Cond, Source, Location, Msg) \
    k_assertsf((Diag), (Cond), ((k_diag_source){ .name = (Source)->name, .text = (Source)->text, .use_byte_offset = true, .byte_offset = (Location) }), Msg)

///===--------------------------------------===///
/// Data types.
///===--------------------------------------===///

/// @brief The maximal alignment Choir, and the @c ch_align_t type, supports.
/// This value is equal to 2 to the 14th power (1 << 14, or 16384) because it is the largest power of two representible with a signed 16-bit twos-complement integer.
/// @ref ch_align_for_bytes
#define CH_ALIGN_MAX (1 << 14)

/// @brief The minimal alignment Choir supports.
/// Nothing can be less aligned than a single byte.
/// @ref ch_align_for_bytes
#define CH_ALIGN_MIN 1

/// @brief Represents the byte size of a piece of data.
/// @ref ch_align_padding
/// @ref ch_align_to
/// @ref ch_align_for_bytes
typedef isize_t ch_size_t;

/// @brief Represents the byte alignment of a piece of data.
/// This should only ever be a power of two, and may produce unexpected results if that is not true.
/// @ref ch_align_padding
/// @ref ch_align_to
/// @ref ch_align_for_bytes
/// @note In a more "type-oriented" language, this would be represented instead as an integer power and converted to the power-of-two when converted to its integer representation at the last moment.
typedef int16_t ch_align_t;

/// @brief Source text from any language or input source.
typedef struct ch_source {
    /// @brief The name of this source, usually a canonical file path for a source file or an angle-bracketted "<compiler-internal>"" name.
    k_string_view name;
    /// @brief The full text of this source.
    k_string_view text;
    /// @brief True if this source represents a "system" file and should be treated more lax by the language semantics.
    /// Used primarily for system C headers, which may make liberal use of extensions or incompatible features.
    bool is_system_source : 1;
} ch_source;

/// @brief A 0-based byte location within the text of a source.
typedef isize_t ch_location;

/// @brief A 0-based byte range within the text of the referenced source.
/// The byte length of this range is given by end - begin.
typedef struct ch_range {
    /// @brief The source this range applies to.
    ch_source* source;
    /// @brief The beginning byte offset of this range.
    ch_location begin;
    /// @brief The ending byte offset of this range.
    ch_location end;
} ch_range;

typedef struct ch_context {
    k_diag* diag;
    k_arena* string_arena;
} ch_context;

///===--------------------------------------===///
/// Size & Alignment API.
///===--------------------------------------===///

/// @brief Returns the minimum number of bytes required (as padding, for example) to add to 'size' such that it, as an offset, is aligned to 'align'.
CHOIR_API ch_size_t ch_align_padding(ch_size_t size, ch_align_t align);

/// @brief Returns 'size' aligned to 'align'.
/// This is achieved by adding the minimum number of padding bytes to 'size' such that it, as an offset, is aligned to 'align'.
CHOIR_API ch_size_t ch_align_to(ch_size_t size, ch_align_t align);

/// @brief Returns the minimum valid alignment to to align something to at least 'bytes' bytes.
/// This will always be the next power of two and never zero.
CHOIR_API ch_align_t ch_align_for_bytes(ch_size_t bytes);

///===--------------------------------------===///
/// Context API.
///===--------------------------------------===///

CHOIR_API void ch_context_init(ch_context* context, k_diag* diag, k_arena* string_arena);

#if defined(__cplusplus)
}
#endif // defined(__cplusplus)

#endif // CHOIR_CORE_H_
