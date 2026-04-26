#ifndef ISH_H
#define ISH_H
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>

typedef struct Ish {
	lua_State *L;
} Ish;

Ish Ish_new();
const char *Ish_get_prompt(Ish *ish);
void Ish_lua_run(Ish *ish, const char *code);
void Ish_reload_config(Ish *ish);
void Ish_shell_main(Ish *ish);
void Ish_dispose(Ish *ish);

#endif
