# Course_Practice_for_Operating_System

杭电操作系统课程实践。参考仓库 https://github.com/leslievan/Operator_System

## 目录

- [项目目录树](#项目目录树)
- [环境要求](#环境要求)
- [实验1：Linux内核模块编程（进程家族与内核线程）](#实验1linux内核模块编程进程家族与内核线程)
- [实验3：Linux进程管理（自定义shell与管道通信，不包含消息队列与共享内存）](#实验3linux进程管理自定义shell与管道通信不包含消息队列与共享内存)
- [实验5：简单文件系统的实现](#实验5简单文件系统的实现)

## 项目目录树

```text
.
├── .gitignore
├── Lab1
│   ├── task
│   │   ├── Makefile
│   │   └── showfamily.c
│   └── thread
│       ├── Makefile
│       └── showthread.c
├── Lab3
│   ├── 3_1
│   │   ├── cmd1.cc
│   │   ├── cmd2.cc
│   │   ├── cmd3.cc
│   │   ├── main.cc
│   │   └── Makefile
│   └── 3_2
│       ├── Makefile
│       ├── mypipe.cc
│       └── pipesize.cc
├── Lab5
│   ├── main.c
│   ├── Makefile
│   ├── myfs.c
│   └── myfs.h
└── README.md
```

## 环境要求

- Linux 6.8.0-107-generic #107~22.04.1-Ubuntu
- make
- gcc / g++
- Lab1 需要内核头文件与 root 权限（用于加载内核模块）

## 实验1：Linux内核模块编程（进程家族与内核线程）

### 名称

- showfamily（Lab1/task）
- showthread（Lab1/thread）

### 效果

- showfamily：输入一个 PID，输出该进程本身、父进程、兄弟进程、子进程信息。
- showthread：遍历并输出系统内核线程信息（名称、PID、状态、优先级、父进程 PID）。

### 复现方法

1. 编译并运行 showfamily

```bash
cd Lab1/task
make
sudo insmod showfamily.ko pid=<目标进程PID>
dmesg | tail -n 80
sudo rmmod showfamily
make clean
```

2. 编译并运行 showthread

```bash
cd Lab1/thread
make
sudo insmod showthread.ko
dmesg | tail -n 80
sudo rmmod showthread
make clean
```

## 实验3：Linux进程管理（自定义shell与管道通信，不包含消息队列与共享内存）

### 名称

- 3_1：简易 Shell（myshell）
- 3_2：管道通信（mypipe、mysize）

### 效果

- 3_1：实现一个基础 shell，支持命令读取、解析、fork + execvp 执行、exit 内建命令。
- 3_2：
	- mypipe：3 个子进程通过同一管道写入，使用命名信号量做互斥，父进程统一读取。
	- mysize：子进程持续向管道写入，用于观察管道写入阻塞现象。

### 复现方法

1. 运行 3_1

```bash
cd Lab3/3_1
make
./myshell
```

在 myshell 中可输入：

```bash
./cmd1
./cmd2
./cmd3
ls
exit
```

2. 运行 3_2

```bash
cd Lab3/3_2
make
./mypipe
./mysize
```

说明：mysize 为持续写入示例，通常需要手动 Ctrl+C 结束。

## 实验5：简单文件系统的实现

### 名称

- myfs（Lab5）

### 效果

- 在用户态实现简化文件系统，支持格式化、目录管理、文件创建删除、打开关闭、读写等命令。
- 启动后提供交互式命令行，磁盘镜像文件为 fsfile。

### 复现方法

1. 编译并运行

```bash
cd Lab5
make
./myfs
```

2. 最小可复现命令序列（在 myfs 交互界面中输入）

```text
format
mkdir testdir
cd testdir
create a.txt
open a.txt
close a.txt
rm a.txt
cd /
close testdir
rmdir testdir
exit
```
