#include "util.h"
#include <ish.h>
#include <unistd.h>

int main(int argc, char **argv) {
	Ish ish = Ish_new();
	if (argc > 1) {
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
