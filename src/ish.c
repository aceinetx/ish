#include "ish.h"
#include "util.h"
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

Ish Ish_new() {
	Ish ish = (Ish){.L = NULL};
	Ish_reload_config(&ish);
	read_history(Ish_get_history_path(&ish));
	return ish;
}

const char *Ish_get_prompt(Ish *ish) {
	const char *prompt = NULL;
	lua_getglobal(ish->L, "prompt");
	if (lua_isnil(ish->L, 1)) {
		prompt = "$ ";
	} else {
		lua_call(ish->L, 0, 1);
		prompt = lua_tostring(ish->L, lua_gettop(ish->L));
		lua_pop(ish->L, 1);
	}
	return prompt;
}

const char *Ish_get_history_path(Ish *ish) {
	static char path[2048];
	snprintf(path, sizeof path, "%s/.ish_history", getenv("HOME"));
	return path;
}

int Ish_get_return_code(Ish *ish) {
	lua_getglobal(ish->L, "ish");
	lua_getfield(ish->L, -1, "returncode");
	lua_pop(ish->L, -2);
	if (lua_isnumber(ish->L, -1)) {
		lua_pop(ish->L, -1);
		return lua_tonumber(ish->L, -1);
	}
	lua_pop(ish->L, -1);
	return 0;
}

void Ish_lua_run(Ish *ish, const char *code) {
	if (luaL_dostring(ish->L, code) != LUA_OK) {
		fprintf(stderr, "lua error: %s\n", lua_tostring(ish->L, -1));
		lua_pop(ish->L, 1); // Remove error message from the stack
	}
}

void Ish_reload_config(Ish *ish) {
	if (ish->L)
		lua_close(ish->L);
	ish->L = luaL_newstate();
	luaL_openlibs(ish->L);
	Ish_lua_openstd(ish);

	Ish_lua_run(ish, (const char[]){
#include <runtime.lua.h>
											 , 0});
	Ish_lua_run(ish, (const char[]){
#include <default.lua.h>
											 , 0});

	char *rc_path = ish_strcat_alloc(getenv("HOME"), "/.ishrc.lua");

	char *rc_code = ish_fs_read(rc_path, NULL);
	if (rc_code) {
		Ish_lua_run(ish, rc_code);

		free(rc_code);
	}

	free(rc_path);
}

void Ish_shell_execline(Ish *ish, const char *s) {
	if (strcmp(s, "exit") == 0) {
		ish->flag = false;
	} else if (strcmp(s, "reload") == 0) {
		Ish_reload_config(ish);
		puts("config reloaded");
	} else if (*s == '!') { // execute lua
		const char *lua_code = s + 1;
		const char *code = lua_pushfstring(ish->L, "return %s", lua_code);

		int status = luaL_loadstring(ish->L, code);
		lua_remove(ish->L, -2);

		if (status != LUA_OK) {
			fprintf(stderr, "lua error: %s\n", lua_tostring(ish->L, -1));
			lua_pop(ish->L, 1);
		}

		if (lua_pcall(ish->L, 0, 1, 0) != LUA_OK) {
			fprintf(stderr, "lua error: %s\n", lua_tostring(ish->L, -1));
			lua_pop(ish->L, 1);
		}

		if (!lua_isnil(ish->L, -1)) {
			printf("%s\n", luaL_tolstring(ish->L, -1, NULL));
			lua_pop(ish->L, 1);
		}
		lua_pop(ish->L, 1);
	} else {
		lua_getglobal(ish->L, "sh");
		if (lua_isnil(ish->L, 1)) {
			puts("no sh function");
			lua_pop(ish->L, 1);
		} else {
			lua_pushstring(ish->L, s);
			lua_call(ish->L, 1, 0);
		}
	}
}

void Ish_shell_main(Ish *ish) {
	ish->flag = true;
	while (ish->flag) {
		const char *prompt = Ish_get_prompt(ish);

		char *input = readline(prompt);

		if (!input)
			break;

		Ish_shell_execline(ish, input);

		add_history(input);

		if (input)
			free(input);
	}
}

void Ish_dispose(Ish *ish) {
	write_history(Ish_get_history_path(ish));
	lua_close(ish->L);
}
