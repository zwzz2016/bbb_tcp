#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAXLINE    4096
#define SERV_PORT  9998    /*�������ݶ˿ںţ���������*/
#define CLIE_PORT  9997    /*�������ݶ˿ںţ��ͻ��ˣ�*/

int main(int argc, char** argv)
{
    int    socket_fd, connect_fd,client_fd;
    struct sockaddr_in servaddr;     /* �������������ַ�ṹ�� */
    struct sockaddr_in clieaddr;     /* �ͻ��˶������ַ�ṹ�� */
    char    buf[MAXLINE],sendbuf[MAXLINE];


   /* �����ͻ����׽���--IPv4Э�飬��������ͨ�ţ�TCPЭ��*/
   if((client_fd=socket(AF_INET,SOCK_STREAM,0))<0)
    {
        perror("socket");
        exit(0);
    }
   /* ��ʼ�� */
  memset(&clieaddr,0,sizeof(clieaddr)); /* ���ݳ�ʼ��-���� */
  clieaddr.sin_family = AF_INET; /* ����IPv4ͨ�� */
  clieaddr.sin_port = htons(CLIE_PORT);/* ���÷������˿ں� */
  /* IP��ַת�������������ʮ����ת��Ϊ������ */
   if( inet_pton(AF_INET, 129.204.181.40, &clieaddr.sin_addr) <= 0)
    {
         printf("inet_pton error for %s\n",129.204.181.40);
         exit(0);
    }
   /* ���׽��ְ󶨵��������������ַ��*/
  if( connect(client_fd, (struct sockaddr *)&clieaddr, sizeof(clieaddr))<0)
    {
        perror("connected failed");
        exit(0);
    }



    /*�������������׽���--IPv4Э�飬��������ͨ�ţ�TCPЭ��*/
    if( (socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
    {
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
    //printf("waiting for client's connection......\n");

   /*����ֱ���пͻ������ӣ���Ȼ���˷�CPU��Դ*/
    if((connect_fd = accept(socket_fd, (struct sockaddr*)NULL, NULL)) <0)
     {
          printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
          exit(1);
     }
        /*���ܿͻ��˴�����������*/
  while((len = recv(connect_fd, buf, MAXLINE, 0))>0)
     {
       buf[len] = '\0';
       //printf("receive message from client: %s\n", buf);
       /*��ͻ��˷��ͻ�Ӧ����*/
       //printf("send message to client: \n");
       //fgets(sendbuf, 4096, stdin);
       if( send(client_fd, buf, strlen(buf), 0) < 0)
         {
            printf("send messaeg error: %s(errno: %d)\n", strerror(errno), errno);
            exit(0);
         }
     }
  close(connect_fd);
  close(socket_fd);
}
