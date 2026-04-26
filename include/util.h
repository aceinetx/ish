#ifndef ISH_UTIL_H
#define ISH_UTIL_H
char *ish_fs_read(const char *filename, long *outSize);

char *ish_strcat_alloc(const char *s, const char *other);
#endif
