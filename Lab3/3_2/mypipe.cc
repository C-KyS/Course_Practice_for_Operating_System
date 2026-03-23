#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>

void ChildrenWrite(int write_fd)
{
    char buffer[1024];
    int cont = 0;
    while (true)
    {
        snprintf(buffer, sizeof(buffer), "This is child process, PID: %d, cont: %d!", getpid(), cont++);
        write(write_fd, buffer, strlen(buffer)); 
        sleep(1); // 每隔1秒写入一次
    }
}

void FatherRead(int read_fd)
{
    char buffer[1024];
    while (true)
    {
        ssize_t n = read(read_fd, buffer, sizeof(buffer)-1);
        if(n > 0)
        {
            buffer[n] = '\0'; 
        }
        std::cout << "Father process read: " << buffer << std::endl;
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
    close(pipefd[1]);        // 关闭写端
    FatherRead(pipefd[0]);   // 从管道中读取数据
    waitpid(id, nullptr, 0); // 等待子进程结束
    close(pipefd[0]);        // 关闭读端
    return 0;
}