#ifndef KOS_H_
#define KOS_H_

#if defined(__cplusplus)
extern "C" {
#endif // defined(__cplusplus)

///===--------------------------------------===///
/// Standard headers.
///===--------------------------------------===///

#include <assert.h>
#include <inttypes.h>
#if __STDC_VERSION__ < 202311L
#    include <stdalign.h>
#endif
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

///===--------------------------------------===///
/// Platform detection.
///===--------------------------------------===///

#ifdef _WIN32
#    define K_WINDOWS 1
#endif // _WIN32

#ifdef __linux__
#    define K_LINUX 1
#    define K_POSIX 1
#endif // __linux__

#ifdef _MSC_VER
#    define K_MSVC 1
#endif // _MSC_VER

#ifdef __clang__
#    define K_CLANG 1
#endif // __clang__

///===--------------------------------------===///
/// "Fancy" macros.
///===--------------------------------------===///

#if __STDC_VERSION__ < 202311L || (defined(_MSC_VER) && !defined(__clang__))
#    define nullptr NULL
#endif

#define k_return_defer(value) \
    do {                      \
        result = (value);     \
        goto defer;           \
    } while (0)

#define k_scope_defer(Expr) \
    for (int _k_scope_defer_##__LINE__ = 0; _k_scope_defer_##__LINE__ == 0; (i++, (Expr)))

#define k_cast(T) (T)
#define k_discard (void)

#define k_assert(Diag, Cond, Msg)                                                                                                                   \
    do {                                                                                                                                            \
        if (!(Cond)) {                                                                                                                              \
            k_diag_emit((Diag), ((k_diag_data){ .level = K_DIAG_FATAL, .message = K_SV_CONST("Assertion failed: " Msg "\nCondition: " #Cond) })); \
        }                                                                                                                                           \
    } while (0)

#define k_assertf(Diag, Cond, Fmt, ...)                                                                                   \
    do {                                                                                                                  \
        if (!(Cond)) {                                                                                                    \
            k_diag_emitf((Diag), K_DIAG_FATAL, "Assertion failed: " Fmt "\nCondition: " #Cond __VA_OPT__(,) __VA_ARGS__); \
        }                                                                                                                 \
    } while (0)

#define k_assertsf(Diag, Cond, Source, Fmt, ...)                                                                                     \
    do {                                                                                                                             \
        if (!(Cond)) {                                                                                                               \
            k_diag_emitsf((Diag), K_DIAG_FATAL, (Source), "Assertion failed: " Fmt "\nCondition: " #Cond __VA_OPT__(,) __VA_ARGS__); \
        }                                                                                                                            \
    } while (0)

#ifndef K_DA_INIT_CAP
/// @brief Initial capacity of a dynamic array.
#    define K_DA_INIT_CAP 256
#endif

/// Declare the required fields of a dynamic array of the given element type.
#define K_DA_DECLARE_INLINE(ElementType) \
    k_arena* arena;                      \
    ElementType* data;                   \
    isize_t capacity;                    \
    isize_t count

// Push an item to a dynamic array.
#define k_da_push(DynArr, Item)                                                                    \
    do {                                                                                           \
        if ((DynArr)->count >= (DynArr)->capacity) {                                               \
            (DynArr)->capacity = (DynArr)->capacity == 0 ? K_DA_INIT_CAP : (DynArr)->capacity * 2; \
            size_t new_size = k_cast(size_t)(DynArr)->capacity * sizeof(*(DynArr)->data);          \
            if ((DynArr)->arena == nullptr) {                                                      \
                (DynArr)->data = realloc((DynArr)->data, new_size);                                \
                memset((DynArr)->data + (DynArr)->count, 0, (DynArr)->capacity - (DynArr)->count); \
            } else {                                                                               \
                void* old_data = (DynArr)->data;                                                   \
                (DynArr)->data = k_arena_alloc((DynArr)->arena, new_size);                         \
                memcpy((DynArr)->data, old_data, k_cast(size_t)(DynArr)->count);                   \
            }                                                                                      \
            assert((DynArr)->data != nullptr && "Buy more RAM lol");                               \
        }                                                                                          \
        (DynArr)->data[(DynArr)->count++] = (Item);                                                \
    } while (0)

#define k_da_free(DynArr)                 \
    do {                                  \
        if ((DynArr)->arena == nullptr) { \
            free((DynArr)->data);         \
            (DynArr)->data = nullptr;     \
            (DynArr)->capacity = 0;       \
        }                                 \
        (DynArr)->count = 0;              \
    } while (0)

#define k_da_ensure_capacity(DynArr, MinCapacity)                                                  \
    do {                                                                                           \
        if ((DynArr)->capacity < (MinCapacity)) {                                                  \
            if ((DynArr)->capacity == 0) {                                                         \
                (DynArr)->capacity = K_DA_INIT_CAP;                                                \
            }                                                                                      \
            while ((MinCapacity) > (DynArr)->capacity) {                                           \
                (DynArr)->capacity *= 2;                                                           \
            }                                                                                      \
            size_t new_size = k_cast(size_t)(DynArr)->capacity * sizeof(*(DynArr)->data);          \
            if ((DynArr)->arena == nullptr) {                                                      \
                (DynArr)->data = realloc((DynArr)->data, new_size);                                \
                memset((DynArr)->data + (DynArr)->count, 0, (DynArr)->capacity - (DynArr)->count); \
            } else {                                                                               \
                void* old_data = (DynArr)->data;                                                   \
                (DynArr)->data = k_arena_alloc((DynArr)->arena, new_size);                         \
                memcpy((DynArr)->data, old_data, k_cast(size_t)(DynArr)->count);                   \
            }                                                                                      \
            assert((DynArr)->data != NULL && "Buy more RAM lol");                                  \
        }                                                                                          \
    } while (0)

// Push several items to a dynamic array.
#define k_da_push_many(DynArr, NewItems, NewItemsCount)                                                                \
    do {                                                                                                               \
        k_da_ensure_capacity((DynArr), (DynArr)->count + (NewItemsCount));                                             \
        memcpy((DynArr)->data + (DynArr)->count, (NewItems), k_cast(size_t)(NewItemsCount) * sizeof(*(DynArr)->data)); \
        (DynArr)->count += (NewItemsCount);                                                                            \
    } while (0)

#define K_STR_FMT       "%.*s"
#define K_STR_EXPAND(S) k_cast(int)(S).count, (S).data

#define K_SV_CONST(cstr) \
    (k_string_view) { .data = cstr, .count = k_cast(isize_t)(sizeof(cstr) / sizeof(char)) - 1 }

///===--------------------------------------===///
/// Data types.
///===--------------------------------------===///

/// @brief Signed analog to @c size_t.
typedef ptrdiff_t isize_t;

/// @brief The distinct diagnostic levels.
typedef enum k_diag_level {
    /// @brief Indicates this diagnostic should be ignored.
    K_DIAG_IGNORE,

    /// @brief The level of a diagnostic which is grouped with the previous non-note diagnostic.
    K_DIAG_NOTE,

    /// @brief Used to report non-critical information to the user, maybe for debug purposes.
    K_DIAG_REMARK,

    /// @brief Used to report potentially unwanted or unintended behavior in the program.
    K_DIAG_WARNING,

    /// @brief Used to report expected or recoverable errors in the program.
    K_DIAG_ERROR,

    /// @brief Indicates a diagnostic reports an unrecoverable state.
    /// After a fatal diagnostic is reported, the program is aborted.
    /// As this terminates the program, no notes can be associated with it by default; this may be configurable in the future.
    /// This can be used to implement formatted asserts.
    K_DIAG_FATAL,
} k_diag_level;

typedef enum k_unicode_decode_result {
    K_UNICODE_SUCCESS = 0,
    K_UNICODE_END_OF_DATA,
    K_UNICODE_OUT_OF_RANGE,
    K_UNICODE_INVALID_START_BYTE,
    K_UNICODE_INVALID_CONTINUE_BYTE,
} k_unicode_decode_result;

/// @brief A single block in an arena.
/// You should not be using this type directly.
typedef struct k_arena_block {
    char* data;
    isize_t count_allocated;
} k_arena_block;

/// @brief A memory arena for controling memory scopes and lifetimes.
typedef struct k_arena k_arena;
struct k_arena {
    K_DA_DECLARE_INLINE(k_arena_block);
};

/// @brief An immutable view into underlying, non-owned string data.
typedef struct k_string_view {
    const char* data;
    isize_t count;
} k_string_view;

/// @brief A mutable, owning string builder.
typedef struct k_string {
    K_DA_DECLARE_INLINE(char);
} k_string;

/// @brief Optional source information for reporting diagnostics within files.
/// None of these fields are required; if empty or zero, they should be ignored when generating the diagnostic output.
/// This means, for example, a binary file could point to a byte_offset without providing any text to display, and the diagnostic callbacks should handle that correctly.
typedef struct k_diag_source {
    /// @brief The name of this source, if available.
    k_string_view name;
    /// @brief The text of this source, if available.
    k_string_view text;

    /// @brief True if the @c byte_offset field is to be used for source location reporting; false otherwise.
    bool use_byte_offset;
    union {
        struct {
            /// @brief The 1-based line into the text of this source.
            /// If no source text is present, this may still be rendered in diagnostics.
            isize_t line;
            /// @brief the 1-based column into the text of this source.
            /// If no source text is present, this may still be rendered in diagnostics.
            isize_t column;
        };

        /// @brief The 0-based byte offset into the contents of this source.
        /// If no source text is present, this may still be rendered in diagnostics.
        /// This is the most logical choice for binary files, for example.
        isize_t byte_offset;
    };
} k_diag_source;

/// @brief Grouped data about a single diagnostic message.
typedef struct k_diag_data {
    /// @brief The level of this diagnostic.
    k_diag_level level;

    /// @brief The optional source of this diagnostic.
    k_diag_source source;

    /// @brief The message of this diagnostic.
    k_string_view message;
} k_diag_data;

/// @brief A dynamic array of diagnostic data, representing a logical group of diagnostics.
typedef struct k_diag_data_group {
    K_DA_DECLARE_INLINE(k_diag_data);
} k_diag_data_group;

/// @brief The type of a diagnostic callback provided to the diagnostic system.
/// Allows implementing custom behavior for diagnostic handling.
typedef void (*k_diag_callback)(void* userdata, k_diag_data_group group);

/// @brief The underlying state of a diagnostic engine.
/// Used to process incoming diagnostic requests and delegate them to a callback.
/// Handles constructing diagnostic groups and tracks things like the error count.
typedef struct k_diag {
    /// @brief The arena to allocate strings into when formatting diagnostics.
    k_arena* string_arena;

    /// @brief User-provided diagnostic callback.
    /// Can accept a userdata pointer which must be set on this struct.
    k_diag_callback callback;

    /// @brief User-provided diagnostic callback userdata.
    /// Will be passed to the callback when a diagnostic group is ready to be processed.
    void* callback_userdata;

    /// @brief The in-progress diagnostic group.
    /// Once a non-note diagnostic would be added to a non-empty group, it is flushed automatically and a new group is started.
    k_diag_data_group diag_group;

    /// @brief Keeps track of the number of errors that have been accepted by the diagnostic engine.
    int32_t error_count;

    /// @brief If non-zero, the maximum number of errors this diagnostic engine can report.
    /// If any more errors are submitted, they are marked as ignored instead.
    /// The first time an error is ignored this way, one final error notifying the user that the error limit has been reached is submitted in its place.
    int32_t error_limit;

    bool has_reported_error_limit_reached;
    bool last_diag_was_ignored;
} k_diag;

typedef struct k_diag_formatted_state {
    FILE* output_stream;
} k_diag_formatted_state;

///===--------------------------------------===///
/// Arenas API.
///===--------------------------------------===///

/// @brief Initialize this allocator to a default state.
/// @param arena The arena to initialize.
void k_arena_init(k_arena* arena);

/// @brief De-initialize this allocator, freeing all of its associated memory.
/// @param arena The arena to de-initialize.
void k_arena_deinit(k_arena* arena);

/// @brief Allocate @c size number of bytes in this arena.
/// The memory will be zeroed and aligned to K_ARENA_ALIGN bytes.
void* k_arena_alloc(k_arena* arena, size_t size);

///===--------------------------------------===///
/// Unicode API.
///===--------------------------------------===///

/// @brief Decodes a UTF-8 codepoint from byte-string data.
/// @param data The input data, assumed to be valid UTF-8 bytes.
/// @param data_count The number of bytes available in the input data.
/// @param offset The offset into the input data to decode from. Should be in the range [0, @c data_count) to attempt decoding.
/// @param out_codepoint If non-null, the memory to populate with the decoded codepoint value.
/// @param out_stride If non-null, the memory to populate with the byte stride of the decoded codepoint value.
/// @return @c K_UNICODE_SUCCESS if a codepoint was successfully decoded, otherwise another @c k_unicode_decode_result value indicating the reason for failure.
k_unicode_decode_result k_utf8_decode(const char* data, isize_t data_count, isize_t offset, int32_t* out_codepoint, isize_t* out_stride);

///===--------------------------------------===///
/// Strings API.
///===--------------------------------------===///

/// @brief Create a @c k_string_view from readonly data and a count.
k_string_view k_sv(const char* data, isize_t count);

/// @brief Create a @c k_string_view from a NUL-terminated C string.
k_string_view k_sv_from_cstr(const char* cstr);

/// @brief Create a sub-string view into the given string view.
k_string_view k_sv_slice(k_string_view sv, isize_t offset, isize_t count);

/// @brief Skip characters until a character @c c is encountered.
/// Returns the skipped characters and modifies the original string view to point at the encountered character, or the end of the string view if it was not found.
k_string_view k_sv_take_until(k_string_view* sv, char c);

/// @brief Appends formatted text to the given string.
void k_sprintf(k_string* s, const char* format, ...);

/// @brief Appends formatted text to the given string.
void k_vsprintf(k_string* s, const char* format, va_list v);

///===--------------------------------------===///
/// Diagnostic API.
///===--------------------------------------===///

void k_diag_formatted(void* userdata, k_diag_data_group group);

void k_diag_init(k_diag* diag, k_arena* string_arena, k_diag_callback callback, void* userdata);
void k_diag_deinit(k_diag* diag);
void k_diag_flush(k_diag* diag);
void k_diag_emit(k_diag* diag, k_diag_data diag_data);
void k_diag_emitf(k_diag* diag, k_diag_level level, const char* format, ...);
void k_diag_emitsf(k_diag* diag, k_diag_level level, k_diag_source source, const char* format, ...);

#if defined(__cplusplus)
}
#endif // defined(__cplusplus)

#endif // KOS_H_
