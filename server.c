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
    struct sockaddr_in     servaddr; /* �������������ַ�ṹ�� */
    //char    buf[MAXLINE],sendbuf[MAXLINE];
    int i,j=0;
    int     len;
    /*�������������׽���--IPv4Э�飬��������ͨ�ţ�TCPЭ��*/
    if( (socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
    printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
    exit(0);
    }
    /*��ʼ��*/
    memset(&servaddr, 0, sizeof(servaddr));/*���ݳ�ʼ��-���� */
    servaddr.sin_family = AF_INET;  /*����IPv4ͨ��*/
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//IP��ַ���ó�INADDR_ANY,��ϵͳ�Զ���ȡ������IP��ַ��
    servaddr.sin_port = htons(SERV_PORT);//���÷������˿�ΪSERV_PORT

    /*�����ص�ַ�󶨵����������׽�����*/
    if( bind(socket_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) <0)
    {
         printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
         exit(0);
    }
    /*��ʼ�����Ƿ��пͻ�������*/
    if( listen(socket_fd, 10) <0)
    {
         printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
         exit(0);
    }
    printf("waiting for client's connection......\n");

   /*����ֱ���пͻ������ӣ���Ȼ���˷�CPU��Դ*/
    if((connect_fd = accept(socket_fd, (struct sockaddr*)NULL, NULL)) <0)
     {
          printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
          exit(1);
     }
        /*���ܿͻ��˴�����������*/
  while((len= recv(connect_fd, j, 3, 0))>0)
     {
      //  buf[len] = '\0';
      //  printf("receive message from client: %s\n", buf);
       /*��ͻ��˷��ͻ�Ӧ����*/
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
