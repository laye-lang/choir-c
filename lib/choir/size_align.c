#include <choir/core.h>

CHOIR_API ch_size_t ch_align_padding(ch_size_t size, ch_align_t align) {
    assert(align >= CH_ALIGN_MIN && align <= CH_ALIGN_MAX);
    return (align - (size % align)) % align;
}

CHOIR_API ch_size_t ch_align_to(ch_size_t size, ch_align_t align) {
    assert(align >= CH_ALIGN_MIN && align <= CH_ALIGN_MAX);
    return size + ch_align_padding(size, align);
}

CHOIR_API ch_align_t ch_align_for_bytes(ch_size_t bytes) {
    /// The absolute maximal alignment we and our 'ch_align_t' type will ever support.
    if (bytes >= CH_ALIGN_MAX) {
        return CH_ALIGN_MAX;
    }

    /// Likewise, the absolute minimal alignment we support, period, is CH_ALIGN_MIN (1).
    if (bytes <= CH_ALIGN_MIN) {
        return CH_ALIGN_MIN;
    }

    /// https://graphics.stanford.edu/%7Eseander/bithacks.html#RoundUpPowerOf2
    ///
    /// Devised by
    ///   - Sean Anderson, Sepember 14, 2001
    ///   - Peter Hart and William Lewis, February 14, 1997
    ///
    /// Computes the next power of two >= 'bytes'.
    bytes--;
    bytes |= bytes >> 1;
    bytes |= bytes >> 2;
    bytes |= bytes >> 4;
    bytes |= bytes >> 8;
    bytes++;

    assert(bytes <= CH_ALIGN_MAX);
    return k_cast(ch_align_t)bytes;
}
