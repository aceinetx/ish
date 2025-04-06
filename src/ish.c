/* clang-format off */
#include <stdio.h>
#include <readline/readline.h>
/* clang-format on */
#include <ish.h>
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

char *prompt = NULL;
char *input = NULL;
bool running = true;

int main(int argc, char **argv) {
	char cwd[255];

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	const char *lua_code = "print('Hello from Lua!')";
	if (luaL_dostring(L, lua_code) != LUA_OK) {
		fprintf(stderr, "Error: %s\n", lua_tostring(L, -1));
		lua_pop(L, 1); // Remove error message from the stack
	}

	lua_close(L);

	return 0;

	prompt = malloc(PROMPT_LENGTH);
	memset(prompt, 0, PROMPT_LENGTH);

	while (running) {
		getcwd(cwd, sizeof(cwd));
		snprintf(prompt, PROMPT_LENGTH, "ish %s $ ", cwd);

		input = readline(prompt);
		if (strcmp(input, "exit") == 0) {
			running = false;
		}

		if (input)
			free(input);
	}
	free(prompt);

	return 0;
}
