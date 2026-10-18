
/*
#include <signal.h>
#include <stdio.h>

#include <readline/readline.h>
int event(void) {
}
void handler(int status) {
	rl_replace_line("", 0);
	// rl_redisplay();
	rl_done = 1;
}

int main(int argc, char **argv) {
	char *entry;
	rl_event_hook = event;
	signal(SIGINT, handler);
	for (;;) {
		entry = readline("> ");

		printf("Entry was: %s\n", entry);
	}
	return 0;
}
*/

#include "util.h"
#include <assert.h>
#include <ish.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *args_shift(int *argc, char ***argv) {
	assert(*argc > 0 && "argc <= 0");
	--(*argc);
	return *(*argv)++;
}

#define args_shift(argc, argv) args_shift(&(argc), &(argv))

int event(void) {
	return 0;
}

void handler(int status) {
	rl_replace_line("", 0);
	rl_done = 1;
}

int main(int argc, char **argv) {
	setenv("SHELL", argv[0], 1);

	/* Setup CTRL+C handler */
	char *entry;
	rl_event_hook = event;
	signal(SIGINT, handler);

	Ish ish = Ish_new();
	read_history(Ish_get_history_path(&ish));

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
		char *code = ish_fs_read(script, NULL);
		if (!code) {
			printf("failed to read %s\n", script);
			goto exit;
		}
		char *code_adjusted = code;
		if (strstr(code, "#!") != NULL) {
			char *p = strchr(code, '\n');
			if (p)
				code_adjusted = p;
		}
		Ish_lua_run(&ish, code_adjusted);
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
	write_history(Ish_get_history_path(&ish));
	Ish_dispose(&ish);

	return 0;
}
