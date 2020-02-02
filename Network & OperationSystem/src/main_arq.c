//
//  main.c
//  ARQ
//
//  Created by 李航 on 2018/12/30.
//  Copyright © 2018 李航. All rights reserved.
//
#include "arq.h"
unsigned int len = sizeof(struct sockaddr);
unsigned int pack_len = sizeof(UDP_pack);

void * ARQ_send(void * p2){
    // 参数转化为可用
    Params param = *(Params *)p2;
    int my_sock = param.sock;
    struct sockaddr *oppo = param.oppo;
    
    UDP_pack pack;
    char buff[100];  // 发送缓存
    
    while (1) {
        memset(buff,0,100);
        printf("请输入你要发送的信息:");
        if (!scanf("%s",  buff)) { break; }
        //计算包的长度，向上取整
        int total = ceil(strlen(buff) * 1.0 / MAX_LENGTH);  
        int winStart = 0,win_size = 0,winEnd = 0;
        int outTime = 0;
        // 确定窗口范围
        updateWindow(&win_size,&winStart,&winEnd,total,NULL);        
        while(winStart <= total){
            if(outTime > 50){
                printf("发生超时！\n");
                break;
            }
            // 发送，发送窗口内的包
            for (int i = winStart; i < winEnd; i++) {               
                // 组装一个包 
                pack2Udp(&pack,buff,winStart,i,total,win_size);      
                // 发送一个包
                sendto(my_sock, &pack, pack_len, 0, oppo, len);     
            }
            // 接受确认号
            long err = recvfrom(my_sock, &pack, pack_len, 0, oppo, &len);   
            // 处理超时以及接收延迟数据
            if(checkTime(&outTime,err,pack,winStart)){ continue; }          
            // 确定窗口范围
            updateWindow(&win_size,&winStart,&winEnd,total,&pack);          
            memset(&pack,0,pack_len);
        }
    }
    return 0;
}



void * ARQ_receive(void * p){
    // 参数转化为可用
    Params param = *(Params *)p;
    int my_sock = param.sock;
    struct sockaddr *oppo = param.oppo;
    
    UDP_pack buff;
    char receive[100];
    // 保证线程存活
    while(1){
        memset(receive,0, sizeof(receive));
        int win_size = WIN_SIZE, winStart = 0;
        int time = 0;
        int total = 1000;
        do{
            // 等待接受数据
            recvfrom(my_sock, &buff, pack_len, 0, oppo, &len);  
            // 模拟发送丢失 重新监听接收
            if (rand()%10 > 8){                      
                memset(&buff,0,pack_len);
            }else{
                // 更新窗口大小
                win_size = buff.win_size;           
                total = buff.total;
                // 赋值
                add2Receive(buff,receive); 
                // 接收次数加一         
                time++;                             
            }
            // 窗口大小次数接收完毕
            if (time >= win_size){                  
                time = 0;
                // 获取确认ack
                ensureACK(&buff,receive);     
                // 窗口移动      
                winStart = buff.ack;                
                // 模拟确认延迟 确认丢失
                if(createError()){ continue; }       
                // 发送确认
                sendto(my_sock, &buff, pack_len, 0, oppo, len);   
            }
        }while (winStart < total);
        if (receive[0] != '\0') {
            printf("对面说:%s\n",receive);
        }
        
    }
    return 0;
}

int main(int argc, const char * argv[]) {
    
    srand((unsigned)time(NULL));
    struct sockaddr_in my,oppo,oppo1;
    pthread_t thread1, thread2;
    Params p,p2;
    
    int my_sock = socket(AF_INET, SOCK_DGRAM, 0);             // 建立本地址描述符并绑定。
    setMySockaddr(&my,NULL,MY);
    bind(my_sock, (struct sockaddr*)&my, sizeof(my));
    
    // 定时器结构体
    typedef struct timeval {
        long tv_sec;
        int tv_usec;
    } Timeval;
    // 设置超时定时器
    Timeval ti;
    ti.tv_sec = 1;
    ti.tv_usec = 0;
    int oppo_sock = socket(AF_INET, SOCK_DGRAM, 0);
    setsockopt(oppo_sock,SOL_SOCKET,SO_RCVTIMEO,&ti, sizeof(ti));
    // ------------发送线程----------------
    setOppoSockaddr(&oppo1,NULL,OPPO);
    setParams(&p2, oppo_sock,(struct sockaddr*)&oppo1);
    pthread_create(&thread2,NULL,ARQ_send,&p2);
    // ------------接收线程----------------
    setOppoSockaddr(&oppo,NULL,OPPO);
    setParams(&p, my_sock,(struct sockaddr*)&oppo);
    pthread_create(&thread1,NULL,ARQ_receive,&p);
    while(1);
    return 0;
}
