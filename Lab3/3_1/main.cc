// #include <iostream>
// #include <cstdio>
// #include <cstdlib>
// #include <cstring>
// #include <string>
// #include <sys/wait.h>
// #include <unistd.h>
// #include <stdlib.h>
// #include <stdio.h>
// #include <string.h>
// #include <time.h>

// // 内建命令函数声明
// int csh_cd(char **args);
// int csh_help(char **args);
// int csh_exit(char **args);
// int csh_hello(char **args);
// int csh_print_info(char **args);

// // 内建命令列表
// char *builtin_str[] = {
//     "cd",
//     "help",
//     "hello",
//     "print_info",
//     "exit"
// };

// //
// int (*builtin_func[]) (char **) = {
//     &csh_cd,
//     &csh_help,
//     &csh_hello,
//     &csh_print_info,
//     &csh_exit
// };

// int csh_num_builtins(void) {
//     return sizeof(builtin_str) / sizeof(char*);
// }

// /*
//  * Builtin function implementations
//  */

// /*
//  * @brief Builtin command: change directory.
//  * @param args List of args. args[0] is "cd". args[1] is the directory.
//  * @return Always returns 1, to continue executing.
//  */
// int csh_cd(char **args)
// {
//     if (args[1] == NULL) {
//         fprintf(stderr, "csh: expected argument to \"cd\"\n");
//     } else {
//         if (chdir(args[1]) != 0) {
//             perror("csh");
//         }
//     }
//     return 1;
// }

// /*
//  * @brief Builtin command: print help.
//  * @param args List of args. Not examined.
//  * @return Always return 1, to continue executing.
//  */
// int csh_help(char **args)
// {
//     int i;
//     printf("A simple shell in C\n");
//     printf("Type program names and arguments, and press enter.\n");
//     printf("The following are built in:\n");

//     for (i = 0; i < csh_num_builtins(); i++) {
//         printf("  %s\n", builtin_str[i]);
//     }

//     printf("Use the man command for information on other programs.\n");
//     return 1;
// }

// /*
//  * @brief Builtin command: print hello and current time.
//  * @param args List of args. Not examined.
//  * @return Always return 1, to continue executing.
//  */
// int csh_hello(char **args)
// {
//     time_t rawtime;
//     struct tm * timeinfo;

//     time(&rawtime);
//     timeinfo = localtime(&rawtime);
//     printf("Hello, Leslie Van.\n");
//     printf("Current local time and date: %s", asctime(timeinfo));
//     return 1;
// }

// /*
//  * @brief Builtin command: print kernel info.
//  * @param args List of args. Not examined.
//  * @return Always return 1, to continue executing.
//  */
// int csh_print_info(char **args)
// {
//     system("uname -a");
//     return 1;
// }

// /*
//  * @brief Builtin command: exit.
//  * @param args List of args. Not examined.
//  * @return Always return 0, to terminate execution.
//  */
// int csh_exit(char **args)
// {
//     return 0;
// }

// /*
//  * @brief Launch a program and wait for it to terminate
//  * @param args Null terminated list of arguments.
//  * @return Always return 1, to continue executing.
//  */
// int csh_launch(char **args)
// {
//     pid_t pid, wpid;
//     int status;

//     pid = fork();
//     if (pid == 0) {
//         // Child process
//         if (execvp(args[0], args) == -1) {
//             perror("csh");
//         }
//         exit(EXIT_FAILURE);
//     } else if (pid < 0) {
//         // Error forking
//         perror("csh");
//     } else {
//         // Parent process
//         do {
//             wpid = waitpid(pid, &status, WUNTRACED);
//         } while (!WIFEXITED(status) && !WIFSIGNALED(status));
//     }

//     return 1;
// }

// /*
//  * @brief Execute shell built-in or launch program.
//  * @param args Null terminated list of arguments.
//  * @return 1 if the shell should continue running, 0 if it should terminate.
//  */
// int csh_execute(char **args)
// {
//     int i;
//     if (args[0] == NULL) {
//         // An empty command was entered
//         return 1;
//     }

//     for (i = 0; i < csh_num_builtins(); i++) {
//         if (strcmp(args[0], builtin_str[i]) == 0) {
//             return (*builtin_func[i])(args);
//         }
//     }

//     // return csh_launch(args);
//     printf("Command not found.\n");
//     return 1;
// }

// /*
//  * @brief Read a line of input from stdin.
//  * @return The line from stdin.
//  */
// char *csh_read_line(void)
// {
//     char *line = NULL;
//     ssize_t bufsize = 0;
//     getline(&line, &bufsize, stdin);
//     return line;
// }

// #define CSH_TOK_BUFSIZE 64
// #define CSH_TOK_DELIM " \t\r\n\a"
// /*
//  * @brief Split a line into tokens.
//  * @param line The line.
//  * @return Null-terminated array of tokens.
//  */
// char **csh_split_line(char *line)
// {
//     int bufsize = CSH_TOK_BUFSIZE, position = 0;
//     char **tokens = malloc(bufsize * sizeof(char*));
//     char *token;

//     if (!tokens) {
//         fprintf(stderr, "csh: allocation error\n");
//         exit(EXIT_FAILURE);
//     }

//     token = strtok(line, CSH_TOK_DELIM);
//     while (token != NULL) {
//         tokens[position] = token;
//         position++;

//         if (position >= bufsize) {
//             bufsize += CSH_TOK_BUFSIZE;
//             tokens = realloc(tokens, bufsize * sizeof(char*));
//             if (!tokens) {
//                 fprintf(stderr, "csh: allocation error\n");
//                 exit(EXIT_FAILURE);
//             }
//         }

//         token = strtok(NULL, CSH_TOK_DELIM);
//     }
//     tokens[position] = NULL;
//     return tokens;
// }

// /*
//  * @brief Loop getting input and execting it.
//  */
// void csh_loop(void)
// {
//     char *line;
//     char **args;
//     int status;

//     do {
//         printf("> ");
//         line = csh_read_line();
//         args = csh_split_line(line);
//         status = csh_execute(args);

//         free(line);
//         free(args);
//     } while (status);
// }

// /*
//  * @brief Main entry point.
//  * @param argc Argument count.
//  * @param argv Argument vector.
//  * @return status code.
//  */
// // int main(int argc, char **argv)
// // {
// //     csh_loop();

// //     return EXIT_SUCCESS;
// // }

//////////////////////////////////////////////////////////////////////////////
// 以下是我的版本
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>

#define COMMAND_SIZE 1024
#define FORMAT "[%s@%s %s]# "
#define MAXARGC 128

// 全局 存储命令行参数
char *g_argv[MAXARGC];
int g_argc = 0;

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

// 环境变量获取pwd
const char *GetPWD()
{
    const char *pwd = getenv("PWD");
    return pwd ? pwd : "None";
}

// 制作命令行提示符
void MakeCommandPrompt(char prompt[], int size)
{
    snprintf(prompt, COMMAND_SIZE, FORMAT, GetUsername(), GetHostname(), GetPWD());
}

// 打印命令行提示符
void PrintCommandPrompt()
{
    char prompt[COMMAND_SIZE];
    MakeCommandPrompt(prompt, sizeof(prompt));
    printf("%s", prompt);
    fflush(stdout);
}

// 获取输入的命令
bool GetCommandLine(char *out, int size)
{
    char *c = fgets(out, size, stdin);
    if (c == NULL)
        return false;

    out[strlen(out) - 1] = 0; // 去掉换行符
    if (sizeof(out) == 0)
        return false;
    return true;
}

// 解析输入命令
bool CommandParse(char *commandline)
{
    // 拆分输入命令, 存入全局变量 g_argv 中
    g_argc = 0;
    g_argv[g_argc++] = strtok(commandline, " ");
    while (g_argv[g_argc++] = strtok(nullptr, " "))
        ;
    g_argc--;
    return g_argc > 0;
}

// 打印命令行参数列表
void PrintArgv()
{
    int i = 0;
    while (g_argv[i])
    {
        printf("argv[%d] = %s\n", i, g_argv[i]);
        i++;
    }
    printf("g_argc = %d\n", g_argc);
}

// 4. (内建命令)检查、执行
bool CheckandExecBuiltin()
{
    if(g_argv[0] == nullptr)
        return false;
        
    std::string cmd(g_argv[0]);
    if (cmd == "exit")
    {
        exit(0);
    }
    return false;
}

// 执行命令
void ExecuteCommand()
{
    pid_t id = fork();
    if (id == 0)
    {
        // 子进程
        // 程序替换
        execvp(g_argv[0], g_argv); // 传入全局参数数组
        fprintf(stderr, "%s: Command not found\n", g_argv[0]);
        exit(1);
    }
    // 父进程
    waitpid(id, nullptr, 0); // 等待子进程结束
}

int main()
{
    while (true)
    { 
        // 1. 输出命令行提示符
        PrintCommandPrompt();

        // 2. 获取输入的命令
        char commandline[COMMAND_SIZE]; // 初始化
        // 获取输入的命令
        if (!(GetCommandLine(commandline, sizeof(commandline))))
            // printf("echo: %s\n", commandline);
            // else
            continue;

        // 3. 解析输入命令
        if (!CommandParse(commandline))
            continue;
        // PrintArgv();

        // 4. (内建命令)检查、执行
        if (CheckandExecBuiltin())
            continue; // 已经执行内建命令, 继续下一轮循环

        // 5. (外部命令)执行命令
        ExecuteCommand();
    }

    return 0;
}
