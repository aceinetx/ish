#ifndef ISH_H
#define ISH_H
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <stdbool.h>

typedef struct Ish {
	lua_State *L;
	bool flag;
} Ish;

Ish Ish_new();
const char *Ish_get_prompt(Ish *ish);
const char *Ish_get_history_path(Ish *ish);
void Ish_lua_run(Ish *ish, const char *code);
void Ish_lua_openstd(Ish *ish);
void Ish_reload_config(Ish *ish);
void Ish_shell_execline(Ish *ish, const char *s);
void Ish_shell_main(Ish *ish);
void Ish_dispose(Ish *ish);

#endif
