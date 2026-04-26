#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *ish_fs_read(const char *filename, long *outSize) {
	FILE *file;
	long fileSize;
	char *buffer;
	int bytesRead;

	if (outSize) {
		*outSize = -1;
	}

	file = fopen(filename, "rb");
	if (!file) {
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	if (fileSize < 0) {
		fclose(file);
		return NULL;
	}

	buffer = (char *)malloc(fileSize + 1);
	if (!buffer) {
		fclose(file);
		return NULL;
	}

	bytesRead = fread(buffer, 1, fileSize, file);
	if (bytesRead != fileSize) {
		free(buffer);
		fclose(file);
		return NULL;
	}
	buffer[fileSize] = 0;

	fclose(file);

	if (outSize) {
		*outSize = fileSize;
	}

	return buffer;
}

char *ish_strcat_alloc(const char *s, const char *other) {
	size_t size = strlen(s);
	size_t other_size = strlen(other);
	size_t new_size = size + other_size + 1;
	char *new = malloc(new_size);
	new[new_size - 1] = 0;
	memcpy(new, s, size);
	memcpy(new + size, other, other_size);
	return new;
}
