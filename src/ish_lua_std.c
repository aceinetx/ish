#include "ish.h"
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

void Ish_lua_openstd(Ish *ish) {
	lua_pushcfunction(ish->L, Ish_lua_std_getcwd);
	lua_setglobal(ish->L, "ish_getcwd");
	lua_pushcfunction(ish->L, Ish_lua_std_chdir);
	lua_setglobal(ish->L, "ish_chdir");
	lua_pushcfunction(ish->L, Ish_lua_std_istermux);
	lua_setglobal(ish->L, "ish_istermux");
	lua_pushcfunction(ish->L, Ish_lua_std_exists);
	lua_setglobal(ish->L, "ish_exists");

	lua_newtable(ish->L);
	lua_setglobal(ish->L, "ish_aliases");
}
