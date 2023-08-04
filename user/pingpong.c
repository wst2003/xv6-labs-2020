#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int fd1[2];
    int fd2[2]; //管道文件描述符，0用于读取，1用于写入
    pipe(fd1);
    pipe(fd2);
    char buffer[16];
    if (fork())
    {
        //parent
        write(fd1[1], "ping", strlen("ping"));
        read(fd2[0], buffer, 4);
        printf("%d: received %s\n", getpid(), buffer);
    }
    else
    {
        //child
        read(fd1[0], buffer, 4);
        printf("%d: received %s\n", getpid(), buffer);
        write(fd2[1], "pong", strlen("pong"));
    }

    exit(0);
}