#include "arena.h"
#include <assert.h>
#include <stdlib.h>

IshArena IshArena_new(size_t size) {
	void *p = malloc(size);
	return (IshArena){.memory = p, .size = size, .head = p};
}

void *IshArena_alloc(IshArena *arena, size_t size) {
	size_t left = arena->memory + arena->size - arena->head - 8;
	if (left < size)
		assert(0 && "No memory left in arena");

	size_t alignment = 8;
	size = (size + alignment - 1) & ~(alignment - 1); // align the size

	void *p = arena->head;
	arena->head += size;
	return p;
}

void IshArena_clear(IshArena *arena) {
	arena->head = arena->memory;
}

void IshArena_dispose(IshArena *arena) {
	IshArena_clear(arena);
	free(arena->memory);
}
