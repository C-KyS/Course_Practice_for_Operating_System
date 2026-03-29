#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "myfs.h"


/** List of builtin commands, followed by their corresponding functions. */
// 可实现的命令列表
char *builtin_str[] = {
        "format",
        "cd",
        "mkdir",
        "rmdir",
        "ls",
        "create",
        "rm",
        "write",
        "read",
        "exit",
        "open",
        "close",
        "pwd"
};

// 对应命令的函数指针数组
int (*builtin_func[])(char **) = {
        &my_format,
        &my_cd,
        &my_mkdir,
        &my_rmdir,
        &my_ls,
        &my_create,
        &my_rm,
        &my_write,
        &my_read,
        &my_exit_sys,
        &my_open,
        &my_close,
        &my_pwd
};

int csh_num_builtins(void) {
    return sizeof(builtin_str) / sizeof(char*);
}
/*
 * @brief Launch a program and wait for it to terminate
 * @param args Null terminated list of arguments.
 * @return Always return 1, to continue executing.
 */
// 外部命令, 
int csh_launch(char **args)
{
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        // 子进程
        if (execvp(args[0], args) == -1) {
            perror("csh");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // fork失败
        perror("csh");
    } else {
        // 父进程
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

/*
 * @brief Execute shell built-in or launch program.
 * @param args Null terminated list of arguments.
 * @return 1 if the shell should continue running, 0 if it should terminate.
 */
// 执行命令, 包括内建命令或外部命令
int csh_execute(char **args)
{
    int i;
    if (args[0] == NULL) {
        // 输入为空
        return 1;
    }

    // 检查是否是内建命令
    for (i = 0; i < csh_num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }

    // 不是内建命令，执行外部命令
    return csh_launch(args);
}

/*
 * @brief Read a line of input from stdin.
 * @return The line from stdin.
 */
// 读取用户输入
char *csh_read_line(void)
{
    char *line = NULL;
    ssize_t bufsize = 0;
    getline(&line, &bufsize, stdin);
    return line;
}

#define CSH_TOK_BUFSIZE 64
#define CSH_TOK_DELIM " \t\r\n\a"
/*
 * @brief Split a line into tokens.
 * @param line The line.
 * @return Null-terminated array of tokens.
 */
// 输入字符串分割
char **csh_split_line(char *line)
{
    int bufsize = CSH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens) {
        fprintf(stderr, "csh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, CSH_TOK_DELIM);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += CSH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "csh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, CSH_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}


// 环境变量获取用户名
const char *GetUsername()
{
    const char *username = getenv("USER");
    return username ? username : "None";
}

// 环境变量获取主机名
const char *GetHostname()
{
    const char *hostname = getenv("HOSTNAME");
    return hostname ? hostname : "None";
}

// 制作命令行提示符
void MakeCommandPrompt(char prompt[], int size)
{
    snprintf(prompt, COMMAND_SIZE, FORMAT, GetUsername(), GetHostname(), current_dir);
}

// 打印命令行提示符
void PrintCommandPrompt()
{
    char prompt[COMMAND_SIZE];
    MakeCommandPrompt(prompt, sizeof(prompt));
    printf("%s", prompt);
    fflush(stdout);
}

// shell 主函数
void csh_loop(void)
{
    char *line;
    char **args;
    int status;

    do {
        // printf("\n\e[1mleslie\e[0m@leslie-PC \e[1m%s\e[0m\n", current_dir);
        PrintCommandPrompt();
        // printf("> \e[032m$\e[0m ");
        line = csh_read_line(); // 读取用户输入
        args = csh_split_line(line); // 输入字符串分割
        status = csh_execute(args); // 执行命令

        free(line);
        free(args);
    } while (status);
}


int main(int argc, char **argv)
{
    start_sys(); // 启动文件系统，加载文件系统
    csh_loop(); // 启动shell

    return EXIT_SUCCESS;
}