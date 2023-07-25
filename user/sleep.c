#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    if(argc==1){
        fprintf(2,"Please input your time!");
        exit(1);
    }
    else if(argc>2){
        fprintf(2,"Args is too much more than needed!");
        exit(2);
    }
    else{
        int sleep_time = atoi(argv[1]);
        sleep(sleep_time);
        exit(0);
    }
}