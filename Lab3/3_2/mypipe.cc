#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

void ChildrenWrite(int write_fd)
{
    
}

void FatherRead(int read_fd)
{
    
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

    // 父进程, 负责读取、
    close(pipefd[1]);        // 关闭写端
    FatherRead(pipefd[0]);   // 从管道中读取数据
    waitpid(id, nullptr, 0); // 等待子进程结束
    close(pipefd[0]);        // 关闭读端
    return 0;
}