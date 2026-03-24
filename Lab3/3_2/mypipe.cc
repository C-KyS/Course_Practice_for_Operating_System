#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <semaphore.h>
#include <fcntl.h>

void ChildrenWrite(int write_fd)
{
    // char buffer[1024];
    // int cont = 0;
    // while (true)
    // {
    //     snprintf(buffer, sizeof(buffer), "This is child process, PID: %d, cont: %d!", getpid(), cont++);
    //     write(write_fd, buffer, strlen(buffer));
    //     // sleep(1); // 每隔1秒写入一次
    // }
    // char c = 0;
    // int count = 0;
    // while (true)
    // {
    //     write(write_fd, &c, sizeof(c));
    //     count++;
    //     std::cout << "child count: " << count << std::endl;
    // }
    const char *msgs[] = {
        "Message from child 1\n",
        "Message from child 2\n",
        "Message from child 3\n"};
}

void FatherRead(int read_fd)
{
    sleep(100);
    char buffer[1024];
    while (true)
    {
        // sleep(5);
        ssize_t n = read(read_fd, buffer, sizeof(buffer) - 1);
        if (n > 0)
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

    // 创建信号量
    const char *sem_name = "/mypipe_write_sem";
    sem_unlink(sem_name); // 清理历史同名信号量
    sem_t *write_sem = sem_open(sem_name, O_CREAT | O_EXCL, 0666, 1);
    if(write_sem == SEM_FAILED)
    {
        std::cerr << "sem_open error" << std::endl;
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