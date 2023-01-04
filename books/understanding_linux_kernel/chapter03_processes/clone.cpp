#include <string.h>
#include <sched.h>
#include <signal.h>
#include <sys/wait.h>

#include <iostream>

using namespace std;

static int child_func(void *arg)
{
    char *buf = static_cast<char *>(arg);
    cout << "Child sees buf = \"" << buf << "\"" << endl;
    strcpy(buf, "hello from child");
}

int main(int argc, char **argv)
{
    const int STACK_SIZE = 65536;
    auto stack = malloc(STACK_SIZE);
    if (!stack)
    {
        perror("malloc");
        exit(1);
    }

    unsigned long flags = 0;
    if (argc > 1 && !strcmp(argv[1], "vm"))
    {
        flags |= CLONE_VM;
    }

    char buf[100];
    strcpy(buf, "hello from parent");
    if (clone(child_func, stack + STACK_SIZE, flags | SIGCHLD, buf) == -1)
    {
        perror("clone");
        exit(1);
    }

    int status;
    if (wait(&status) == -1)
    {
        perror("wait");
        exit(1);
    }

    cout << "Child exited with status " << status << ". buf = \"" << buf << "\"" << endl;

    return 0;
}
