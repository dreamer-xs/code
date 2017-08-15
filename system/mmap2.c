#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/mman.h>




int main(int argc, char **argv)
{
    int ret = 0;
    char *_start = NULL;
    char buf[100] = {0};

    struct stat f_stat;
    memset(&f_stat, 0, sizeof(f_stat));
    memset(buf, 0, 100);

    if(argc < 2)
    {
        fprintf(stderr, "Usage: %s %s\n", argv[0], argv[1]);
        return -1;
    }

    int fd = open(argv[1],O_RDWR);
    if(fd < 0)
    {
        perror("open");
        return -1;
    }


    scanf("%s", buf);

    _start = mmap(NULL, f_stat.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    
    /*
    while(f_stat.st_size > 0)
    {
        ret = fputs(_start, stdout);
        f_stat.st_size -=ret;
    }
    */

    printf("**********************%s\n",buf);
    memcpy(_start, buf, 200);

    munmap(_start, f_stat.st_size);

    close(fd);


    return 0;
}

