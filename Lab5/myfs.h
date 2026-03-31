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
#define END 0xffff       /**FAT中的文件结束标志 */
#define FREE 0x0000      /**FAT中的盘块空闲标志 */
#define ROOT "/"         /**根目录*/
#define ROOT_BLOCK_NUM 2 /**< 根目录初始所占盘块总数 */
#define MAX_OPENFILE 10  /**< 最多同时打开文件个数 */
#define NAMELENGTH 32
#define PATHLENGTH 128
#define DELIM "/"
#define FOLDER_COLOR "\e[1;32m"
#define DEFAULT_COLOR "\e[0m"
#define WRITE_SIZE 20 * BLOCK_SIZE

/**
 * 引导块BLOCK0
 * 存放虚拟磁盘相关信息
 * 比如磁盘块大小、磁盘块数量等
 */
typedef struct BLOCK0
{
    char information[200];      // 存储一些描述信息, 如磁盘块大小、磁盘块数量等
    unsigned short root;        /**根目录文件的起始盘块号 */
    unsigned char *start_block; /**< 虚拟磁盘上数据区开始位置 */
} block0;

/**
 * FCB
 * 文件静态信息表
 */
typedef struct FCB
{
    char filename[8];          // 文件名
    char exname[3];            // 文件扩展名
    unsigned char attribute;   // 文件属性, 0x00为文件, 0x01为目录
    unsigned char reserve[10]; //
    unsigned short time;       // 文件创建时间
    unsigned short date;       // 文件创建日期
    unsigned short first;      // 文件起始盘块号
    unsigned long length;      // 文件长度
    char free;                 // 文件是否被占用, 0为占用, 1为未占用
} fcb;

/**
 * FAT.
 * 记录下一个磁盘块的盘号.
 * END值代表文件结束, FREE值代表盘块空闲.
 */
typedef struct id
{
    unsigned short id;
} fat;

/**
用户打开文件表
 */
typedef struct USEROPEN
{
    /** FCB. */
    fcb open_fcb;

    /** 文件使用动态信息 */
    char dir[80];   // 打开文件路径
    int count;      // 读写指针位置
    char fcb_state; // FCB是否被修改过, 0为未修改, 1为修改过
    char free;      // 打开表项是否为空, 若为0 则为空, 1为不空
} useropen;

/** 全局变量 */
extern unsigned char *fs_head;               // 虚拟磁盘内存区的起始地址
extern useropen openfile_list[MAX_OPENFILE]; /**用户打开文件表useropen数组 */
extern int curdir;                           // 当前文件描述符, 即当前目录的FCB在openfile_list中的下标
extern char current_dir[80];                 // 当前文件路径, 例: /home/hy2/
extern unsigned char *start;                 /**虚拟磁盘数据区的起始地址 */

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