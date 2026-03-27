#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <semaphore.h>
#include <fcntl.h>

void ChildrenWrite(int write_fd)
{
    char c = 'A';

    int cnt = 0;
    while (true)
    {
        // 写入数据到管道
        write(write_fd, &c, sizeof(c));
        std::cout << "Child process write, cnt = " << cnt++ << std::endl;
    }
}

void FatherRead(int read_fd)
{
    sleep(100); // 等待子进程写入数据
    char buffer[1024];
    while (true)
    {
        ssize_t n = read(read_fd, buffer, sizeof(buffer) - 1);
        if (n > 0)
        {
            buffer[n] = '\0';
        }
        std::cout << buffer << std::endl;
    }
}

int main()
{
    // 创建管道
    int pipefd[2] = {0};
    if (pipe(pipefd) == -1)
    {
        std::cerr << "pipe create error" << std::endl;
        return 1;
    }

    // 创建子进程
    pid_t id = fork();
    if (id == 0)
    {
        // 子进程, 负责写入
        close(pipefd[0]); // 关闭读端

        ChildrenWrite(pipefd[1]); // 向管道中写入数据

        close(pipefd[1]); // 关闭写端
        exit(0);
    }

    // 父进程, 负责读取
    close(pipefd[1]); // 关闭写端

    // 阻塞等待子进程死亡

    waitpid(id, nullptr, 0); // 等待子进程结束

    FatherRead(pipefd[0]); // 从管道中读取数据

    close(pipefd[0]); // 关闭读端
    return 0;
}