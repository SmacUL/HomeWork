//
//  upd.h
//  stopAndWait
//
//  Created by 李航 on 2018/12/30.
//  Copyright © 2018 李航. All rights reserved.
//

#ifndef upd_h
#define upd_h

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX_LENGTH 12

#define OPPO 9999
#define MY  8888

// 停止等待协议
typedef struct udp_pack{
    unsigned int seq;          // 序列号&确认号
    unsigned int total;
    char content[MAX_LENGTH];  // 包内容最长十二字节
} Udp_pack;

// 用于向线程传递参数
typedef struct params{
    int sock;
    struct sockaddr* oppo;
} Params;

// 发送
void pack2Udp(int i,int total,char * buff,Udp_pack * pack);  // 打包一个udp数据包
void checkTime(int * outTime,long * err,Udp_pack pack,int i);  // 处理延迟确认 没有确认的异常

// 接收
int add2Receive(Udp_pack buff,char * receive);               // 添加到接收缓存并返回确认号
int createError(void);                                       // 模拟确认延迟 确认丢失
int isThisMsg(int seq,Udp_pack pack);                        // 判定是不是这次应该接受的信息
// 主
void setOppoSockaddr(struct sockaddr_in * oppo,char * ipAddr,int port);
void setMySockaddr(struct sockaddr_in * my,char * ipAddr,int port);
void setParams(Params * p,int sock,struct sockaddr* addr);

/**
 * 将缓冲区中的数据写入到包中，一次处理一个包
 */ 
void pack2Udp(int i,int total,char * buff,Udp_pack * pack){
    // 序列号
    pack->seq = i;
    // 总包数量
    pack->total = total;
    for(int j = 0;j < MAX_LENGTH; j++){
        pack->content[j] = buff[i * MAX_LENGTH + j];
    }
}

/**
 * 发送方，用于判断接收方是否接收到正确的数据
 * 判断接收到的 seq 是否是正确的。
 */ 
void checkTime(int * outTime, long * err, Udp_pack pack, int i){
    // 如果有延迟,则重发(接受的确认号是之前的)
    if (pack.seq != i){
        (*err) = -1;
        (* outTime)++;
    } else if (*err == -1){ // 没有确认(超时)
        (* outTime)++;
    }
}

/**
 * 接收包中的数据写入到接收方缓冲区的指定位置
 */ 
int add2Receive(Udp_pack buff, char * receive){
    int i = buff.seq;
    // 如果没有收到过，则赋值
    if(receive[i*MAX_LENGTH]=='\0'){
        for(int j = 0; j < MAX_LENGTH; j++){
            receive[i * MAX_LENGTH + j] = buff.content[j];
        }
    }
    return i;
}

/**
 * 创造延迟或丢失的情况
 */
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
 * 
 */ 
int isThisMsg(int seq, Udp_pack pack){
    if (seq + 1 != pack.seq) {
        return 0;
    }
    return 1;
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

#endif /* upd_h */
