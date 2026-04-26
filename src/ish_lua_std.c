#include "ish.h"
#include <unistd.h>

static int Ish_lua_std_getcwd(lua_State *L) {
	char cwd[512];
	getcwd(cwd, 512);

	lua_pushstring(L, cwd);
	return 1;
}

void Ish_lua_openstd(Ish *ish) {
	lua_pushcfunction(ish->L, Ish_lua_std_getcwd);
	lua_setglobal(ish->L, "ish_getcwd");
}
