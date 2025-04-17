#ifndef CHOIR_MACROS_H_
#define CHOIR_MACROS_H_

#if __STDC_VERSION__ < 202311L || (defined(_MSC_VER) && !defined(__clang__))
#    define nullptr NULL
#endif

#define cast(T) (T)

#define return_defer(value) \
    do {                    \
        result = (value);   \
        goto defer;         \
    } while (0)

/// Initial capacity of a dynamic array.
#ifndef DA_INIT_CAP
#    define DA_INIT_CAP 256
#endif

/// Declare the required fields of a dynamic array of the given element type.
#define DA_DECLARE_INLINE(da_element_type) \
    da_element_type* data;                 \
    isize capacity;                        \
    isize count

/// Declare a dynamic array type with the given name and element type.
#define DA_DECLARE(da_name, da_element_type) \
    typedef struct da_name {                 \
        DA_DECLARE_INLINE(da_element_type)   \
    } da_name

// Push an item to a dynamic array.
#define da_push(da, item)                                                                               \
    do {                                                                                                \
        if ((da)->count >= (da)->capacity) {                                                            \
            (da)->capacity = (da)->capacity == 0 ? DA_INIT_CAP : (da)->capacity * 2;                    \
            (da)->data = ch_realloc((da)->allocator, (da)->data, (da)->capacity * sizeof(*(da)->data)); \
            assert((da)->data != NULL && "Buy more RAM lol");                                           \
        }                                                                                               \
        (da)->data[(da)->count++] = (item);                                                             \
    } while (0)

#define da_free(da) ch_dealloc((da)->allocator, (da)->data)

// Push several items to a dynamic array.
#define da_push_many(da, new_items, new_data_count)                                                     \
    do {                                                                                                \
        if ((da)->count + (new_data_count) > (da)->capacity) {                                          \
            if ((da)->capacity == 0) {                                                                  \
                (da)->capacity = DA_INIT_CAP;                                                           \
            }                                                                                           \
            while ((da)->count + (new_data_count) > (da)->capacity) {                                   \
                (da)->capacity *= 2;                                                                    \
            }                                                                                           \
            (da)->data = ch_realloc((da)->allocator, (da)->data, (da)->capacity * sizeof(*(da)->data)); \
            assert((da)->data != NULL && "Buy more RAM lol");                                           \
        }                                                                                               \
        memcpy((da)->data + (da)->count, (new_items), (new_data_count) * sizeof(*(da)->data));          \
        (da)->count += (new_data_count);                                                                \
    } while (0)

#define STR_FMT       "%.*s"
#define STR_EXPAND(S) cast(int)(S).count, (S).data

#define SV_CONST(cstr) \
    (ch_string_view) { .data = cstr, .count = cast(isize)(sizeof(cstr) / sizeof(char)) - 1 }

#endif // CHOIR_MACROS_H_
