#include <ish.h>

int main(int argc, char **argv) {
	Ish ish = Ish_new();
	Ish_shell_main(&ish);
	Ish_dispose(&ish);

	return 0;
}
