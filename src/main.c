#include "util.h"
#include <assert.h>
#include <ish.h>
#include <unistd.h>

char *args_shift(int *argc, char ***argv) {
	assert(*argc > 0 && "argc <= 0");
	--(*argc);
	return *(*argv)++;
}

#define args_shift(argc, argv) args_shift(&(argc), &(argv))

int main(int argc, char **argv) {
	Ish ish = Ish_new();
	const char *script = NULL;

	args_shift(argc, argv);
	while (argc) {
		const char *arg = args_shift(argc, argv);
		if (!strcmp(arg, "-l")) {
			// todo
		} else {
			script = arg;
		}
	}

	if (script) {
		char *code = ish_fs_read(argv[1], NULL);
		if (!code) {
			puts("failed to read script");
			goto exit;
		}
		Ish_lua_run(&ish, code);
		free(code);
	} else {
		if (isatty(STDIN_FILENO)) {
			Ish_shell_main(&ish);
		} else {
			char *line = NULL;
			size_t len = 0;

			while (getline(&line, &len, stdin) != -1) {
				Ish_shell_execline(&ish, line);
			}

			free(line);
		}
	}

exit:
	Ish_dispose(&ish);

	return 0;
}
