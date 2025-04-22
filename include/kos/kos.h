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

#define k_cast(T) (T)

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
#define K_STR_EXPAND(S) ch_cast(int)(S).count, (S).data

#define K_SV_CONST(cstr) \
    (k_string_view) { .data = cstr, .count = k_cast(isize_t)(sizeof(cstr) / sizeof(char)) - 1 }

///===--------------------------------------===///
/// Data types.
///===--------------------------------------===///

typedef ptrdiff_t isize_t;

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
/// Strings API.
///===--------------------------------------===///

k_string_view k_sv(const char* data, isize_t count);
k_string_view k_sv_from_cstr(const char* cstr);
k_string_view k_sv_slice(k_string_view sv, isize_t offset, isize_t count);

void k_sprintf(k_string* s, const char* format, ...);
void k_vsprintf(k_string* s, const char* format, va_list v);

#if defined(__cplusplus)
}
#endif // defined(__cplusplus)

#endif // KOS_H_
