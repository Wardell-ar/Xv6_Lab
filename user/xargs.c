// Lab Xv6 and Unix utilities
// xargs.c

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAXN 1024

int
main(int argc, char *argv[])
{
    // 如果参数个数小于 2
    if (argc < 2) {
        // 打印参数错误提示
        fprintf(2, "usage: xargs command\n");
        // 异常退出
        exit(1);
    }
    // 存放子进程 exec 的参数
    char * argvs[MAXARG];
    // 索引
    int index = 0;
    // 略去 xargs ，用来保存命令行参数
    for (int i = 1; i < argc; ++i) {
        argvs[index++] = argv[i];
    }
    // 缓冲区存放从管道读出的数据
    char buf[MAXN] = {"\0"};
    
    int n;
    
	// 0 代表的是管道的 0，也就是从管道循环读取数据
    while((n = read(0, buf, MAXN)) > 0 ) {
        //printf("buf:%s\n",buf);
        // 临时缓冲区存放追加的参数
		char temp[MAXN] = {"\0"};
        // xargs 命令的参数后面再追加参数
        argvs[index] = temp;
        // 内循环获取追加的参数并创建子进程执行命令
        for(int i = 0; i < strlen(buf); ++i) {
            // 读取单个输入行，当遇到换行符时，创建子线程
            if(buf[i] == '\n') {
                // printf("%s\n",argvs[index]);
                //printf("\nparam:%s\n",argvs[index]);
                // 创建子线程执行命令
                if (fork() == 0) {
                    exec(argv[1], argvs);
                }
                
                // 等待子线程执行完毕
                wait(0);
                argvs[index++] = &temp[i+1];
                // printf("%s\n",&temp[i+1]);
            } else {
                // 否则，读取管道的输出作为输入
                temp[i] = buf[i];
                //printf("%c",temp[i]);
            }
        }
    }
    // 正常退出
    exit(0);
}
// #include "kernel/types.h"
// #include "kernel/stat.h"
// #include "kernel/param.h"
// #include "user/user.h"


// int
// main(int argc, char* argv[]) 
// {
//   char buf[512];
//   char* args[MAXARG];
//   int nargs = 0;
//   int i;

//   //读取标准输入的每行
//   while (1) 
//   {
//     //逐字符读取直到换行符
//     int n = read(0, buf, sizeof(buf));
//     if (n <= 0)
//       break;
//     nargs = 0;
//     //保存xargs的命令及其参数
//     for (int i = 1; i < argc; ++i) 
//     {
//       args[nargs++] = argv[i];
//     }
//     //写入其他参数
//     char temp[512];
//     args[nargs++] = temp;
//     for (i = 0; i < n; i++)
//     {
//       if (buf[i] == '\n') 
//       {
//         buf[i] = '\0';  //终止参数字符串
//         args[nargs++] = buf + i + 1; //下一个参数从 '\0' 后开始
//       }
//       else
//       {
//         temp[i] = buf[i];
//       }
//     }

//     //创建子进程执行命令
//     if (fork() == 0) 
//     {
//       exec(argv[1], args);
//     }
//     else 
//       wait(0); // 等待子进程完成
//   }

//   exit(0);
// }