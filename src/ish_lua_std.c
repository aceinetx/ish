#include "ish.h"
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>

static int Ish_lua_std_getcwd(lua_State *L) {
	char cwd[512];
	getcwd(cwd, 512);

	lua_pushstring(L, cwd);
	return 1;
}

static int Ish_lua_std_chdir(lua_State *L) {
	const char *s = lua_tostring(L, 1);
	chdir(s);

	lua_pushnil(L);
	return 1;
}

static int Ish_lua_std_istermux(lua_State *L) {
	const char *termux_version = getenv("TERMUX_VERSION");
	lua_pushboolean(L, termux_version != NULL);
	return 1;
}

static int Ish_lua_std_exists(lua_State *L) {
	const char *path = lua_tostring(L, 1);

	lua_pushboolean(L, access(path, F_OK) == 0);
	return 1;
}

static int Ish_lua_std_setenv(lua_State *L) {
	const char *name = lua_tostring(L, 1);
	const char *value = lua_tostring(L, 2);

	setenv(name, value, 1);
	return 1;
}

void Ish_lua_openstd(Ish *ish) {
	lua_newtable(ish->L);

	const lua_CFunction functions[] = {Ish_lua_std_getcwd, Ish_lua_std_chdir, Ish_lua_std_istermux, Ish_lua_std_exists, Ish_lua_std_setenv};
	const char *names[] = {"getcwd", "chdir", "istermux", "exists", "setenv"};
	assert(sizeof functions / sizeof functions[0] == sizeof names / sizeof names[0]);
	for (int i = 0; i < sizeof functions / sizeof functions[0]; i++) {
		lua_pushcfunction(ish->L, functions[i]);
		lua_setfield(ish->L, -2, names[i]);
	}

	lua_pushnumber(ish->L, 0);
	lua_setfield(ish->L, -2, "returncode");
	lua_newtable(ish->L);
	lua_setfield(ish->L, -2, "aliases");

	lua_setglobal(ish->L, "ish");
}
