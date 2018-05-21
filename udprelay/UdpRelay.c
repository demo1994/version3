//
// Created by ddd on 18-5-21.
//

#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include <pthread.h>
#include<string.h>
#include <stdlib.h>

struct udpRelay {
    char *DEFAULT_IP;
    int DEFAULT_BUFFER_LENGTH;
    int DEFUALT_PORT;

    char *sendIp;
    char *recvIp;
    int sendPort;
    int recvPort;
    char *writetxt;
    int sockfd;
}test;
   void udpRelayCons()
    {
        test.DEFAULT_IP="127.0.0.1";
        test.DEFAULT_BUFFER_LENGTH=2048;
        test.DEFUALT_PORT=8888;

    }

    void setRecvIp(char*ip)
    {
        test.recvIp=ip;

    }
    void setRecvPort(int port)
    {
        test.recvPort=port;
    }
    void setWritetxt(char *txt)
    {
        test.writetxt=txt;
    }

    void init()
    {
        test.sockfd=socket(AF_INET,SOCK_DGRAM,0);
        //设置recvIP和port
        setRecvIp(test.DEFAULT_IP);
        setRecvPort(test.DEFUALT_PORT);
        setWritetxt("ddddddd");
        struct sockaddr_in addr;
        addr.sin_family =AF_INET;
        addr.sin_port =htons(test.recvPort);
        addr.sin_addr.s_addr= inet_addr(test.recvIp);
        //pthread_t write_id;
        //绑定socket对象与通信链接
        int ret =bind(test.sockfd,(struct sockaddr*)&addr,sizeof(addr));
        if(0>ret)
        {
            printf("bind error\n");

            exit(0);

        }
    }

    void *write_func(void *args)
    {
        //char writebuff[MAXLINE];
        char *write = test.writetxt;
        //int fd=socket(AF_INET,SOCK_DGRAM,0);
        struct sockaddr_in addr;
        addr.sin_family =AF_INET;
        addr.sin_port =htons(test.recvPort);
        addr.sin_addr.s_addr = inet_addr(test.recvIp);
        //fd = *(int*)arg;
        //memset(&writebuff,0,sizeof(writebuff));

        //signal(SIGPIPE,signal_pipe); /* 注册 SIGPIPE信号 */
        while(1)
        {
            sleep(1);
            sendto(test.sockfd,write,
               strlen(write),0,(struct sockaddr*)&addr,sizeof(addr));/*向客户端发送数据*/
        }
    }

    void *read_func(void *args)
    {
        struct sockaddr_in cli;
        socklen_t len=sizeof(cli);
        char buf[256];
        while(1)
        {

            memset(&buf, 0, sizeof(buf));
            recvfrom(test.sockfd, buf, 256, 0, (struct sockaddr *) &cli, &len);
            printf("server recv data: %s \n", buf);
            sleep(1);
        }
    }


int main()
{
    udpRelayCons();
    init();

    pthread_t write_id,read_id;
    if(pthread_create(&write_id,NULL,write_func,NULL))/*创建 write 线程*/
    {
        printf("pthread_create write_func err\n");
    }


    if(pthread_create(&read_id,NULL,read_func,NULL))/*创建 read 线程*/
    {
        printf("pthread_create read_func err\n");
    }
    while(1)
    {}

    return 0;

}