#include <kos/kos.h>

#define K_ARENA_ALIGN      16
#define K_ARENA_BLOCK_SIZE (8 * 1024 * 1024)

void k_arena_init(k_arena* arena) {
    *arena = (k_arena){0};
}

void k_arena_deinit(k_arena* arena) {
    for (isize_t i = 0; i < arena->count; i++) {
        k_arena_block* block = &arena->data[i];
        free(block->data);
    }

    k_da_free(arena);
}

void* k_arena_alloc(k_arena* arena, size_t size) {
    isize_t aligned_size = k_cast(isize_t)(size + (K_ARENA_ALIGN - (size % K_ARENA_ALIGN)) % K_ARENA_ALIGN);
    assert(aligned_size > 0);
    assert(aligned_size <= K_ARENA_BLOCK_SIZE);

    k_arena_block* block = nullptr;
    for (isize_t i = 0; block == nullptr && i < arena->count; i++) {
        if (K_ARENA_BLOCK_SIZE - arena->data[i].count_allocated >= aligned_size) {
            block = &arena->data[i];
        }
    }

    if (nullptr == block) {
        char* block_memory = malloc(K_ARENA_BLOCK_SIZE);
        k_da_push(arena, (k_arena_block){.data = block_memory});
        assert(arena->count > 0);
        block = &arena->data[arena->count - 1];
    }

    char* result = block->data + block->count_allocated;
    memset(result, 0, aligned_size);
    block->count_allocated += aligned_size;
    return result;
}
