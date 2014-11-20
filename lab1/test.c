#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>


int main(void)
{
    char **argv = {NULL};
    setreuid(1337, 1337);
    execve("/bin/sh", argv, NULL);
}
