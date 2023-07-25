#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    char *fork_err_msg = "fork failed\n";

    int main_pipe[2];
    pipe(main_pipe);

    int child_pid = fork();

    if (child_pid < 0)
    {
        write(2, fork_err_msg, strlen(fork_err_msg));
        exit(1);
    }

    if (child_pid == 0)
    {
        // This is the child process
        int prime = 0;
        close(main_pipe[1]);
        while (1)
        {
            if (0 == read(main_pipe[0], &prime, sizeof(prime)))
            {
                close(main_pipe[0]);
                exit(0);
            }

            printf("prime %d\n", prime);

            int child_pipe[2];
            pipe(child_pipe);
            int grandchild_pid = fork();
            if (grandchild_pid < 0)
            {
                write(2, fork_err_msg, strlen(fork_err_msg));
                exit(1);
            }

            // Parent (child of the first process)
            if (grandchild_pid > 0)
            {
                int num = 0;
                close(child_pipe[0]);
                while (read(main_pipe[0], &num, sizeof(num)) != 0)
                {
                    if (num % prime != 0)
                    {
                        write(child_pipe[1], &num, sizeof(num));
                    }
                }
                close(main_pipe[0]);
                close(child_pipe[1]);
                wait(0);
                exit(0);
            }

            // Child (grandchild of the first process)
            close(main_pipe[0]);
            close(child_pipe[1]);
            main_pipe[0] = child_pipe[0];
        }
    }

    // This is the first process
    close(main_pipe[0]);
    for (int i = 2; i <= 35; i++)
    {
        write(main_pipe[1], &i, sizeof(i));
    }
    close(main_pipe[1]);
    wait(0);

    exit(0);
}
