#include <kos/kos.h>
#include <stdio.h>

k_string_view k_sv(const char* data, isize_t count) {
    return (k_string_view){
        .data = data,
        .count = count,
    };
}

k_string_view k_sv_from_cstr(const char* cstr) {
    return (k_string_view){
        .data = cstr,
        .count = k_cast(isize_t) strlen(cstr),
    };
}

k_string_view k_sv_slice(k_string_view sv, isize_t offset, isize_t count) {
    assert(offset >= 0);
    assert(offset + count < sv.count);

    return (k_string_view){
        .data = sv.data + offset,
        .count = count,
    };
}

k_string_view k_sv_take_until(k_string_view* sv, char c) {
    k_string_view result = {
        .data = sv->data,
        .count = 0,
    };

    while (sv->count > 0 && sv->data[0] != c) {
        sv->data++;
        sv->count--;
        result.count++;
    }

    return result;
}

void k_sprintf(k_string* s, const char* format, ...) {
    assert(s != nullptr);
    assert(format != nullptr);

    va_list v;
    va_start(v, format);
    k_vsprintf(s, format, v);
    va_end(v);
}

void k_vsprintf(k_string* s, const char* format, va_list v) {
    assert(s != nullptr);
    assert(format != nullptr);

    va_list v1;
    va_copy(v1, v);
    int buffer_count = vsnprintf(NULL, 0, format, v1);
    va_end(v1);

    k_da_ensure_capacity(s, s->count + buffer_count + 1);
    va_list v2;
    va_copy(v2, v);
    int written = vsnprintf(s->data + s->count, k_cast(size_t) buffer_count + 1, format, v2);
    va_end(v2);
    s->count += buffer_count;

    assert(written == buffer_count);
    assert(s->capacity > s->count);
    assert(s->data[s->count] == 0);
}
