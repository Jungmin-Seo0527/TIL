#include <stdio.h>
#include <unistd.h>
#include <wait.h>

int value = 5;

int main()
{
    int i;
    pid_t pid;
    for (i = 0; i < 3; i++)
    {
        pid = fork();
        value += 5;
        printf("Hello, fork()! pid = %d, value = %d\n", pid, value);
    }
    // printf("Hello, fork()! pid = %d, value = %d\n", pid, value);
    return 0;
}