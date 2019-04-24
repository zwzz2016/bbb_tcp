#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#define SERV_PORT 9998
#define MAXLINE 3

int main(int argc, char** argv)
{
    int    socket_fd, connect_fd;
    struct sockaddr_in     servaddr; /* 服务器端网络地址结构体 */
    //char    buf[MAXLINE],sendbuf[MAXLINE];
    int i,j=0;
    int     len;
    /*创建服务器端套接字--IPv4协议，面向连接通信，TCP协议*/
    if( (socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
    printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
    exit(0);
    }
    /*初始化*/
    memset(&servaddr, 0, sizeof(servaddr));/*数据初始化-清零 */
    servaddr.sin_family = AF_INET;  /*设置IPv4通信*/
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//IP地址设置成INADDR_ANY,让系统自动获取本机的IP地址。
    servaddr.sin_port = htons(SERV_PORT);//设置服务器端口为SERV_PORT

    /*将本地地址绑定到所创建的套接字上*/
    if( bind(socket_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) <0)
    {
         printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
         exit(0);
    }
    /*开始监听是否有客户端连接*/
    if( listen(socket_fd, 10) <0)
    {
         printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
         exit(0);
    }
    printf("waiting for client's connection......\n");

   /*阻塞直到有客户端连接，不然多浪费CPU资源*/
    if((connect_fd = accept(socket_fd, (struct sockaddr*)NULL, NULL)) <0)
     {
          printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
          exit(1);
     }
        /*接受客户端传过来的数据*/
  while((len= recv(connect_fd, j, 3, 0))>0)
     {
      //  buf[len] = '\0';
      //  printf("receive message from client: %s\n", buf);
       /*向客户端发送回应数据*/
      //  printf("send message to client: \n");
      //  fgets(sendbuf, 4096, stdin);
      j=j*j;
       if( send(connect_fd, j, strlen(j), 0) < 0)
         {
            printf("send messaeg error: %s(errno: %d)\n", strerror(errno), errno);
            exit(0);
         }
     }
  close(connect_fd);
  close(socket_fd);
}
