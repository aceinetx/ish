#include "execute.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void ish_execute(char **args) {
	pid_t pid = fork();
	if (pid < 0) {
		return;
	} else if (pid == 0) {
		execvp(args[0], args);
		return;
	} else {
		int status;
		waitpid(pid, &status, 0);
	}
}
