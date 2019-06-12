#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <unistd.h>
#define SERV_PORT 9997
#define MAXLINE 4096

int main(int argc, char** argv)
{
    int    socket_fd, connect_fd;
    struct sockaddr_in     servaddr; /* 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷址锟结构锟斤拷 */
    char    buf[MAXLINE],sendbuf[MAXLINE];
    int num=0;
    int     len;
    /*锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟� 锟斤拷锟�--IPv4协锟介，锟斤拷锟斤拷锟斤拷锟斤拷通锟脚ｏ拷TCP协锟斤拷*/
    if( (socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
    printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
    exit(0);
    }
    /*锟斤拷始锟斤拷*/
    memset(&servaddr, 0, sizeof(servaddr));/*锟斤拷锟捷筹拷始锟斤拷-锟斤拷锟斤拷 */
    servaddr.sin_family = AF_INET;  /*锟斤拷锟斤拷IPv4通锟斤拷*/
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//IP锟斤拷址锟斤拷锟矫筹拷INADDR_ANY,锟斤拷系统锟皆讹拷锟斤拷取锟斤拷锟斤拷锟斤拷IP锟斤拷址锟斤拷
    servaddr.sin_port = htons(SERV_PORT);//锟斤拷锟矫凤拷锟斤拷锟斤拷锟剿匡拷为SERV_PORT

    /*锟斤拷锟斤拷锟截碉拷址锟襟定碉拷锟斤拷锟斤拷锟� 锟斤拷锟斤拷锟�*/
    if( bind(socket_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) <0)
    {
         printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
         exit(0);
    }
    /*锟斤拷始锟斤拷锟斤拷锟角凤拷锟叫客伙拷锟斤拷锟斤拷锟斤拷*/
    if( listen(socket_fd, 10) <0)
    {
         printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
         exit(0);
    }
    printf("waiting for client's connection......\n");

   /*锟斤拷锟斤拷直锟斤拷锟叫客伙拷锟斤拷锟斤拷锟接ｏ拷锟斤拷然锟斤拷锟剿凤拷CPU锟斤拷源*/
    if((connect_fd = accept(socket_fd, (struct sockaddr*)NULL, NULL)) <0)
     {
          printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
          exit(1);
     }
        /*锟斤拷锟杰客伙拷锟剿达拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷*/
  while((len= recv(connect_fd, buf, MAXLINE, 0))>0)
     {
       buf[len] = '\0';
       printf("receive message from client: %s\n", buf);
       /*锟斤拷锟酵伙拷锟剿凤拷锟酵伙拷应锟斤拷锟斤拷*/
       // printf("send message to client: \n");
       // fgets(sendbuf, 4096, stdin);
       //if( send(connect_fd, sendbuf, strlen(sendbuf), 0) < 0)
       //  {
       //     printf("send messaeg error: %s(errno: %d)\n", strerror(errno), errno);
       //     exit(0);
       //  }
     }
  close(connect_fd);
  close(socket_fd);
}
