#include <unistd.h>

void execute(char **args) {
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
