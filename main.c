# include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv)
{


    int fd1 = open("1", O_CREAT);
    int fd2 = open("2", O_CREAT);
    int fd3 = open("3", O_CREAT);
    printf("%d\n", fd3);
    close(fd3);
    close(fd2);
    int fd4 = open("4", O_CREAT);
    printf("%d\n", fd4);
    int fd5 = open("5", O_CREAT);



    
    return 0;
}