#include "kernel/types.h"
#include "user/user.h"

 
#define MAXNUM 36
 
void prime(int read_p,int write_p){
    char nums[MAXNUM];
    read(read_p,nums,MAXNUM);//读取上一级传过来的数组
    int target = 0;
    for(int i=0;i<MAXNUM;i++){
        //不断对数字进行判断，找出当前第一个质数
        if(nums[i]=='0'){
            target = i;
            break;//跳出循环
        }
    }
    if(target==0){
        exit(0);
    }
    printf("prime %d\n",target);
    nums[target]='1';
    //筛去当前质数的公倍数
    for(int i=0;i<MAXNUM;i++){
        if(i%target==0 ){
            nums[i] = '1';
        }
    }
    
 
    //创建子进程，进行流水
    int pid  = fork();
    if(pid>0){
        //当前进程
        write(write_p,nums,MAXNUM);
        wait(0);
    }
    else if(pid==0){
        //当前进程的子进程
        prime(read_p,write_p);
        wait(0);
    }
}
 
int
main(int argc,int *argv[]){
    //创建一个数组，用于判断质数
    //下标i表示要判断的数字，num[i]=1表示已经判断,num[i]=0表示未判断
    char nums[MAXNUM];
    //数组的初始化
    memset(nums,'0',sizeof(nums));
 
    //管道的创建
    int p[2];
    pipe(p);
 
    int pid = fork();
    //父进程写nums数组
    if(pid>0){
        nums[0] = '1';
        nums[1] = '1';//0,1已经判断
        write(p[1],nums,MAXNUM);
        wait(0);
    }
    //子进程调用prime函数进行下一级的判断
    else if(pid==0){
        prime(p[0],p[1]);//递归地使用相同的管道，减少内存损耗，通过wait()实现父子进程顺序执行
        wait(0);
    }
    exit(0);

}