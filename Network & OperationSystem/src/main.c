//
//  main.c
//  stopAndWait
//
//  Created by 李航 on 2018/12/30.
//  Copyright © 2018 李航. All rights reserved.
//

#include "upd.h"

unsigned int pack_len = sizeof(Udp_pack);
unsigned int len = sizeof(struct sockaddr);

void * stop_and_wait_send(void * p2){
    // 参数转化为可用
    Params param = *(Params *)p2;
    int my_sock = param.sock;
    struct sockaddr *oppo = param.oppo;
    
    Udp_pack pack;
    char buff[100];      // 发送缓存
    while (1) {          // 一次完整的发送
        memset(buff,0,100);
        printf("请输入您要发送的信息:");
        if(!scanf("%s", buff)){ break; }
        int outTime = 0;  // 设定超时次数
        //这一次内容的总包数  向下取整
        int total = (int) strlen(buff) / MAX_LENGTH; 
        for(int i=0;i <= total; i++){
            pack2Udp(i, total, buff, &pack);
            long err;
            do{
                // 发送这一个包
                sendto(my_sock, &pack, pack_len, 0, oppo, len);           
                // 等待确认
                err = recvfrom(my_sock, &pack, pack_len,0,oppo,&len);     
                // 检查是否有异常
                checkTime(&outTime,&err,pack,i);                          
            }while(err == -1&&(outTime < 20));
            memset(&pack, 0, pack_len);
            if (outTime > 20) {  // 在跳出一次,跳出本次发送。
                break;
            }
        }
    }
    return 0;
}

void * stop_and_wait_receive(void * p){
    // 参数转化为可用
    Params param = *(Params *)p;
    int my_sock = param.sock;
    struct sockaddr *oppo = param.oppo;

    Udp_pack pack;
    char receive[100];
    while(1){            // 保证线程存活
        memset(receive,0, sizeof(receive));
        int seq = -1;
        // 保证一次所有信息都收到
        while (1) {                                           
            memset(&pack, 0, pack_len);
            // 等待接受数据
            recvfrom(my_sock, &pack, pack_len, 0, oppo, &len);
            // 模拟发送丢失 重新监听接收
            if (rand()%10 > 8) { continue; }            
            // 判断是否是这次发送的信息      
            if (!isThisMsg(seq, pack)){ continue; }      
            // 接受确认号      
            int i = add2Receive(pack, receive);          
            // 模拟确认延迟 确认丢失      
            if(createError()){ continue; }                 
            // 发送确认信息
            sendto(my_sock, &pack, pack_len, 0, oppo, len);   
            printf("确认号%d\n",pack.seq);
            if (i == pack.total) { 
                break;
            } else { 
                seq++;
            } // 如果最后一个都收到了就跳出循环,否则序列号+1继续接收
        } // 一次完整的接收循环
        printf("对面说:%s\n",receive);
    }
}

int main(int argc, const char * argv[]) {
    srand((unsigned)time(NULL));
    struct sockaddr_in my,oppo,oppo1;
    pthread_t thread1, thread2;
    Params p,p2;
    
    int my_sock = socket(AF_INET, SOCK_DGRAM, 0);             // 建立本地址描述符并绑定。
    setMySockaddr(&my, NULL, MY);
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
    setsockopt(oppo_sock,SOL_SOCKET, SO_RCVTIMEO, &ti, sizeof(ti));
    // ------------发送线程----------------
    setOppoSockaddr(&oppo1, NULL, OPPO);
    setParams(&p2, oppo_sock, (struct sockaddr*)&oppo1);
    pthread_create(&thread2, NULL, stop_and_wait_send, &p2);
    // ------------接收线程----------------
    setOppoSockaddr(&oppo, NULL, OPPO);
    setParams(&p, my_sock, (struct sockaddr*)&oppo);
    pthread_create(&thread1, NULL, stop_and_wait_receive, &p);
    while(1);
    return 0;
}
