#include "kernel/types.h"
#include "user/user.h"

int main() {
    int pipe1[2]; // 父进程到子进程的管道
    int pipe2[2]; // 子进程到父进程的管道

    // 创建管道
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        printf("pipe_error");
        exit(1);
    }

    int pid = fork();
    if (pid < 0) {
        printf("fork_error");
        exit(1);
    }

    if (pid == 0) {
        // 子进程
        close(pipe1[1]); // 关闭pipe1的写端
        close(pipe2[0]); // 关闭pipe2的读端

        char buf;
        read(pipe1[0], &buf, 1); // 从父进程读取字节
        printf("%d: received ping\n", getpid());
        write(pipe2[1], &buf, 1); // 将字节写回给父进程

        close(pipe1[0]);
        close(pipe2[1]);
    } else {
        // 父进程
        close(pipe1[0]); // 关闭pipe1的读端
        close(pipe2[1]); // 关闭pipe2的写端

        char byte = 'x';
        write(pipe1[1], &byte, 1); // 将字节发送给子进程
        read(pipe2[0], &byte, 1); // 从子进程读取字节
        printf("%d: received pong\n", getpid());

        close(pipe1[1]);
        close(pipe2[0]);
    }

   exit(0);
}
