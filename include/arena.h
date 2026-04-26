#ifndef ISH_ARENA_H
#define ISH_ARENA_H
#include <stddef.h>
#include <stdint.h>

typedef struct IshArena {
	char *memory;
	size_t size;
	char *head;
} IshArena;

IshArena IshArena_new(size_t size);
void *IshArena_alloc(IshArena *arena, size_t size);
void IshArena_clear(IshArena *arena);
void IshArena_dispose(IshArena *arena);

#endif
