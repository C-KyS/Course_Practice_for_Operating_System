#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <semaphore.h>
#include <fcntl.h>

void ChildrenWrite(int write_fd, sem_t *write_sem, int child_index)
{
    char buffer[1024];
    snprintf(buffer, sizeof(buffer), "[This is child process %d, PID: %d]", child_index, getpid());

    // write_sem信号量实现互斥
    if (sem_wait(write_sem) == -1) {
        perror("sem_wait"); return;
    }

    // 写入数据到管道
    write(write_fd, buffer, strlen(buffer));

    // 释放信号量
    // 释放信号量（解锁）
    if (sem_post(write_sem) == -1) {
        perror("sem_post");
    }
}

void FatherRead(int read_fd)
{
    char buffer[1024];
    // while (true)
    // {
        ssize_t n = read(read_fd, buffer, sizeof(buffer) - 1);
        if (n > 0)
        {
            buffer[n] = '\0';
        }
        std::cout << "Father process read: " << buffer << std::endl;
    // }
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
    if (write_sem == SEM_FAILED)
    {
        std::cerr << "sem_open error" << std::endl;
        return 1;
    }

    // 创建3个子进程
    pid_t pids[3] = {0};

    for (int i = 0; i < 3; i++)
    {
        pid_t id = fork();
        if (id == 0)
        {
            // 子进程, 负责写入
            close(pipefd[0]); // 关闭读端

            ChildrenWrite(pipefd[1], write_sem, i); // 向管道中写入数据

            close(pipefd[1]); // 关闭写端
            exit(0);
        }
        else if (id > 0)
        { // 父进程, 记录子进程PID
            pids[i] = id;
        }
        else
        {
            std::cerr << "fork error" << std::endl;
            return 1;
        }
    }

    // 父进程, 负责读取
    close(pipefd[1]); // 关闭写端

    // 阻塞等待子进程死亡
    std::cout << "Parent is waiting for all 3 children to finish writing..." << std::endl;
    for (int i = 0; i < 3; ++i)
    {
        if (pids[i] > 0)
        {
            waitpid(pids[i], nullptr, 0); // 等待子进程结束
        }
    }
    std::cout << "All children exited. Parent starts reading the pipe:" << std::endl;
    FatherRead(pipefd[0]); // 从管道中读取数据

    close(pipefd[0]); // 关闭读端

    // 清理信号量
    sem_close(write_sem);
    sem_unlink(sem_name);
    return 0;
}