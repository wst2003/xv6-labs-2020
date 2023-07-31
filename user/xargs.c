#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

// Executes a command with given arguments.
void execute_command(char *command, char *args[]) {
    if (fork()) {
        // Parent process waits for the child process to finish.
        wait(0);
    } else {
        // Child process executes the command.
        exec(command, args);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(2, "Error: too few parameters for xargs\n");
        exit(1);
    }

    int i;
    int args_count = 0;
    char *args[MAXARG];
    for (i = 1; i < argc; ++i) {
        args[args_count++] = argv[i];
    }
    int base_args_count = args_count;
    char ch;
    char *line;
    char buffer[512];
    line = buffer;
    int index = 0;
    while (read(0, &ch, 1) > 0) {
        if (ch == '\n') {
            line[index] = '\0';
            index = 0;

            args[args_count++] = line;
            args[args_count] = 0;

            execute_command(argv[1], args);

            args_count = base_args_count;
        } else if (ch == ' ') {
            line[index] = '\0';
            index = 0;
            args[args_count++] = line;
            char new_buffer[512];
            line = new_buffer;
        } else {
            line[index++] = ch;
        }
    }
    exit(0);
}
