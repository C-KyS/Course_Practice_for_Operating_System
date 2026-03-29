/**
 * @file    simplefs.h
 * @brief   Setup in FAT16 file system.
 * @details Macro definitions, structs such as FCB and FAT, and some global variable.
 * @author  Leslie Van
 * @date    2018-12-19 to 2019-1-3
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#ifndef OPERATOR_SYSTEM_EXP4_SIMPLEFS_H
#define OPERATOR_SYSTEM_EXP4_SIMPLEFS_H
#define COMMAND_SIZE 1024
#define FORMAT "[%s@%s \e[1;34m%s\e[0m ]> \e[032m$\e[0m " // 定义命令行提示符格式

#define BLOCK_SIZE 1024
#define BLOCK_NUM 1024
#define DISK_SIZE 1048576 // 定义模拟磁盘大小为1MB
#define SYS_PATH "./fsfile"
#define END 0xffff       /**< End of the block, a flag in FAT. */
#define FREE 0x0000      /**< Unused block, a flag in FAT. */
#define ROOT "/"         /**< Root directory name.*/
#define ROOT_BLOCK_NUM 2 /**< Block of the initial root directory. */
#define MAX_OPENFILE 10  /**< Max files to open at the same time. */
#define NAMELENGTH 32
#define PATHLENGTH 128
#define DELIM "/"
#define FOLDER_COLOR "\e[1;32m"
#define DEFAULT_COLOR "\e[0m"
#define WRITE_SIZE 20 * BLOCK_SIZE

/**
 * @brief Store virtual disk information.
 * Contain info like block size, block count and some other information about disk.
 */
typedef struct BLOCK0
{
    char information[200];
    unsigned short root;        /**< Block number of the root directory. */
    unsigned char *start_block; /**< Location of the first data block. */
} block0;

/**
 * @brief File control block.
 * Store file info both the description and current state.
 */
typedef struct FCB
{
    char filename[8];
    char exname[3];
    unsigned char attribute; /**< 0: directory or 1: file. */
    unsigned char reserve[10];
    unsigned short time;  /**< File create time. */
    unsigned short date;  /**< File create date. */
    unsigned short first; /**< First block num of the file. */
    unsigned long length; /**< Block count of the file. */
    char free;
} fcb;

/**
 * @brief File allocation table.
 * Record the next block num of file.
 * When value is 0xffff, this block is the last block of the file.
 */
typedef struct FAT
{
    unsigned short id;
} fat;

/**
 * @brief A file entry opened by user.
 * Contain file control block and current state.
 */
typedef struct USEROPEN
{
    /** FCB. */
    fcb open_fcb;
    /** Current state. */
    char dir[80]; // 
    int count;
    char fcb_state;
    char free;
} useropen;

/** Global variables. */
extern unsigned char *fs_head;               // 虚拟磁盘内存区的起始地址
extern useropen openfile_list[MAX_OPENFILE]; /**< File array opened by user. */
extern int curdir;                           // 当前文件描述符, 即当前目录的FCB在openfile_list中的下标
extern char current_dir[80];                 // 当前文件路径, 例: /home/hy2/
extern unsigned char *start;                 /**< Location of the first data block. */

int start_sys(void); // 启动文件系统，加载文件系统

int my_format(char **args);

int do_format(void);

int my_cd(char **args);

void do_chdir(int fd);

int my_pwd(char **args);

int my_mkdir(char **args);

int do_mkdir(const char *parpath, const char *dirname);

int my_rmdir(char **args);

void do_rmdir(fcb *dir);

int my_ls(char **args);

void do_ls(int first, char mode);

int my_create(char **args);

int do_create(const char *parpath, const char *filename);

int my_rm(char **args);

void do_rm(fcb *file);

int my_open(char **args);

int do_open(char *path);

int my_close(char **args);

void do_close(int fd);

int my_write(char **args);

int do_write(int fd, char *content, size_t len, int wstyle);

int my_read(char **args);

int do_read(int fd, int len, char *text);

int my_exit_sys();

int get_free(int count);

int set_free(unsigned short first, unsigned short length, int mode);

int set_fcb(fcb *f, const char *filename, const char *exname, unsigned char attr, unsigned short first,
            unsigned long length,
            char ffree);

unsigned short get_time(struct tm *timeinfo);

unsigned short get_date(struct tm *timeinfo);

fcb *fcb_cpy(fcb *dest, fcb *src);

char *get_abspath(char *abspath, const char *relpath);

int get_useropen();

fcb *find_fcb(const char *path);

fcb *find_fcb_r(char *token, int root);

void init_folder(int first, int second);

void get_fullname(char *fullname, fcb *fcb1);

char *trans_date(char *sdate, unsigned short date);

char *trans_time(char *stime, unsigned short time);

#endif // OPERATOR_SYSTEM_EXP4_SIMPLEFS_H