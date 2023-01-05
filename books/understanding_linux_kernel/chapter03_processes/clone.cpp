#include <string.h>
#include <sched.h>
#include <signal.h>
#include <sys/wait.h>

#include <iostream>

using namespace std;

// https://eli.thegreenplace.net/2018/launching-linux-threads-and-processes-with-clone/
// https://bugzilla.kernel.org/show_bug.cgi?id=203105

volatile pid_t child_tid = 0;
volatile pid_t parent_tid = 0;

static int child_func(void *arg)
{
    char *buf = static_cast<char *>(arg);
    printf("Child sees buf = \"%s\"\n", buf);
    strcpy(buf, "hello from child");
    printf("1Child process pid %d.\n", child_tid);
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

    flags |= CLONE_CHILD_SETTID;
    flags |= CLONE_PARENT_SETTID;

    char buf[100];
    strcpy(buf, "hello from parent");
    if (clone(child_func, stack + STACK_SIZE, flags | SIGCHLD, buf, &parent_tid, NULL, &child_tid) == -1)
    {
        perror("clone");
        exit(1);
    }

    // cout << "Child pid: " << parent_tid << endl;
    printf("2Child process pid %d.\n", parent_tid);

    int status;
    if (wait(&status) == -1)
    {
        perror("wait");
        exit(1);
    }

    printf("Child exited with status %d. buf = \"%s\"\n", status, buf);

    return 0;
}
