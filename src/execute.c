#include "execute.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void ish_execute(char **args) {
	pid_t pid = fork();
	if (pid < 0) {
		return;
	} else if (pid == 0) {
		// Child process: use default signal handling
		signal(SIGINT, SIG_DFL);
		execvp(args[0], args);
		return;
	} else {
		// Parent process: ignore SIGINT while waiting
		signal(SIGINT, SIG_IGN);
		int status;
		waitpid(pid, &status, 0);
		// Restore default signal handling
		signal(SIGINT, SIG_DFL);
	}
}
