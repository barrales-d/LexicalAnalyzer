#ifndef __ARENA_H__
#define __ARENA_H__

#include <stdint.h>
#include <stdlib.h>

#define DEFAULT_ARENA_CAP 1024

struct Arena {
    size_t size;
    size_t capacity;
    void* items;
};
typedef struct Arena Arena;

Arena arena_make();
void* arena_reserve(Arena* arena, size_t amount);
void  arena_free(Arena* arena);

#endif //   __ARENA_H__

// #define ARENA_IMPLEMENTATION
#ifdef ARENA_IMPLEMENTATION
Arena arena_make()
{
    Arena a;
    a.size = 0;
    a.capacity = DEFAULT_ARENA_CAP;
    a.items = malloc(a.capacity);
    return a;
}
void* arena_reserve(Arena* arena, size_t amount)
{
    
    size_t next_size = arena->size + amount;
    if(next_size >= arena->capacity) {
        arena->capacity = arena->capacity + arena->capacity / 2;
        arena->items = realloc(arena->items, arena->capacity);
    }
    void* ret_val = arena->items + arena->size;
    arena->size = next_size;
    return ret_val;
}
void  arena_free(Arena* arena)
{
    free(arena->items);
    arena->items = NULL;
    arena->size = 0;
    arena->capacity = 0;
}
#endif