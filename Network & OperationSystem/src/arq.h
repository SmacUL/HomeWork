//
//  arq.h/Users/lh_os/Desktop/课程设计/ARQ/ARQ/main.c
//  ARQ
//
//  Created by 李航 on 2018/12/30.
//  Copyright © 2018 李航. All rights reserved.
//

#ifndef arq_h
#define arq_h

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>


// 包的长度
#define MAX_LENGTH 4
// 窗口大小
#define WIN_SIZE 3

#define OPPO 9999
#define MY  8888

// 连续ARQ协议
typedef struct udp_pack{
    unsigned int seq;          // 序列号
    unsigned int total;        // 包的个数
    unsigned int win_size;     // 窗口大小
    unsigned int ack;          // 确认号
    unsigned int winStart;     // 窗口开始
    unsigned int new;
    char content[MAX_LENGTH];  //内容最长十字节
}UDP_pack;

// 用于向线程传递参数
typedef struct params{
    int sock;
    struct sockaddr* oppo;
}Params;

// 发送
void pack2Udp(UDP_pack * pack,char * buff,int winStart,int i,int total,int win_size);
void updateWindow(int * win_size, int * winStart,int * winEnd,int total,UDP_pack * pack);
int checkTime(int * outTime,long err,UDP_pack pack,int winStart);
//接收
void add2Receive(UDP_pack buff,char * receive);
int createError(void);
void ensureACK(UDP_pack * pack,char * receive);
// 主
void setOppoSockaddr(struct sockaddr_in * oppo,char * ipAddr,int port);
void setMySockaddr(struct sockaddr_in * my,char * ipAddr,int port);
void setParams(Params * p,int sock,struct sockaddr* addr);


/**
 * 打包数据
 */ 
void pack2Udp(UDP_pack * pack,char * buff,int winStart,int i,int total,int win_size){
    for(int z = 0; z < MAX_LENGTH; z++){
        pack->content[z] = buff[i * MAX_LENGTH + z];
    }
    pack->winStart = winStart;
    pack->seq = i;
    pack->total = total;
    pack->win_size = win_size;
}

/**
 * 更新窗口信息
 */ 
void updateWindow(int * win_size, int * winStart,int * winEnd,int total, UDP_pack * pack){
    int remainderLength = total;
    * win_size = WIN_SIZE;
    if (pack != NULL) {
        remainderLength = total - pack->ack;         // 发送窗口的大小
        *winStart = pack->ack;                       // [winStart, winEnd)
    }
    if (remainderLength < WIN_SIZE) {
        *win_size = remainderLength;
    }
    *winEnd =*winStart + *win_size;
}

/**
 * 如果有异常情况将 outTime 加 1
 */ 
int checkTime(int * outTime,long err,UDP_pack pack,int winStart){
    
    if(err == -1){           // 没有确认(超时)
        (* outTime)++;
    }else if(pack.ack < winStart){ // 如果有延迟,则重发(接受的确认号是之前的)
        (* outTime)++;
    }else{                   // 正常
        return 0;
    }
    return 1;
}

/**
 * 将收到的数据包中的数据写入输出缓冲区内
 */ 
void add2Receive(UDP_pack buff,char * receive){
    // 如果没有收到过，则赋值
    int i = buff.seq;
    if(receive[i * MAX_LENGTH]=='\0'){
        for(int j = 0;j < MAX_LENGTH;j++){
            receive[i * MAX_LENGTH + j] = buff.content[j];
        }
    }
}


int createError(){
    if (rand()%10 > 8) {  // 确认延迟
        sleep(3);
    }
    if (rand()%10 > 8) {  // 确认丢失
        return 1;
    }
    return 0;
}

/**
 * 计算确认号
 * 即找出接收方收到的序列正确的最后一个数据包的 seq 值。
 */ 
void ensureACK(UDP_pack * pack,char * receive){
    int ack = pack->winStart;
    for (int i = pack->winStart; i < pack->winStart + pack->win_size;i++){
        if (receive[i * MAX_LENGTH] == '\0') {
            break;
        }else{
            ack++;
        }
    }
    pack->ack = ack;
}




void setOppoSockaddr(struct sockaddr_in * oppo,char * ipAddr,int port){
    memset(oppo, 0, sizeof(&oppo));
    oppo->sin_family = AF_INET;
    if (ipAddr == NULL) {
     oppo->sin_addr.s_addr = htonl(INADDR_ANY);
    }else{
     oppo->sin_addr.s_addr = inet_addr(ipAddr);
    }
    oppo->sin_port = htons(port);
}

void setMySockaddr(struct sockaddr_in * my,char * ipAddr,int port){
    memset(my, 0, sizeof(&my));
    my->sin_family = AF_INET;
    if (ipAddr == NULL) {
        my->sin_addr.s_addr = htonl(INADDR_ANY);
    }else{
        my->sin_addr.s_addr = inet_addr(ipAddr);
    }
    my->sin_port = htons(port);
}

void setParams(Params * p,int sock,struct sockaddr* addr){
    p->sock = sock;
    p->oppo = addr;
}

#endif /* arq_h */
