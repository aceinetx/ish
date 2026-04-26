#include "util.h"
#include <stdio.h>
#include <stdlib.h>

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

	buffer = (char *)malloc(fileSize);
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

	fclose(file);

	if (outSize) {
		*outSize = fileSize;
	}

	return buffer;
}
