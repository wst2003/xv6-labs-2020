#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
int find(char *path, char *target_filename);
char* extract_filename(char *path) {
  char *p;

  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  return p;
}

void process_file(char *path, char *target_filename) {
    if(0 == strcmp(extract_filename(path), target_filename)) {
        fprintf(1, "%s\n", path);
    }
}

void process_directory(int fd, char *path, char *target_filename) {
    char buffer[512], *buffer_ptr;
    struct stat st;
    struct dirent de;

    strcpy(buffer, path);
    buffer_ptr = buffer + strlen(buffer);
    *buffer_ptr++='/';

    while(read(fd, &de, sizeof(de)) == sizeof(de)) {
        if(de.inum == 0 || 0 == strcmp(".", de.name) || 0 == strcmp("..", de.name)) {
            continue;
        }
        memmove(buffer_ptr, de.name, DIRSIZ);
        buffer_ptr[DIRSIZ] = 0;
        if(stat(buffer, &st) < 0) {
            printf("find: cannot stat %s\n", buffer);
            continue;
        }

        find(buffer, target_filename);
    }
}

int find(char *path, char *target_filename) {  
    int fd;
    struct stat st;

    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return -1;
    }

    if(fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return -1;
    }

    switch(st.type) {
        case T_FILE:
            process_file(path, target_filename);
            break;
        case T_DIR:
            process_directory(fd, path, target_filename);
            break;
    }

    close(fd);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(2, "not enough arguments\n");
        exit(1);
    }

    find(argv[1], argv[2]);
    exit(0);
}
