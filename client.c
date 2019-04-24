#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>

#define MAXLINE    4096
#define SERV_PORT  9998

int main(int argc, char *argv[])
        {
            char sendbuf[MAXLINE],receivebuf[MAXLINE];
            struct sockaddr_in servaddr;
            int client_sockfd;
            int rec_len;
            /* �ж����������Ĳ����Ƿ���ȷ */
             if( argc != 2)
             {
                 printf("usage: ./client <ipaddress>\n");
                 exit(0);
              }
             /* �����ͻ����׽���--IPv4Э�飬��������ͨ�ţ�TCPЭ��*/
             if((client_sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
              {
                  perror("socket");
                  exit(0);
              }
             /* ��ʼ�� */
            memset(&servaddr,0,sizeof(servaddr)); /* ���ݳ�ʼ��-���� */
            servaddr.sin_family = AF_INET; /* ����IPv4ͨ�� */
            servaddr.sin_port = htons(SERV_PORT);/* ���÷������˿ں� */
            /* IP��ַת�������������ʮ����ת��Ϊ������ */
             if( inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
              {
                   printf("inet_pton error for %s\n",argv[1]);
                   exit(0);
              }
             /* ���׽��ְ󶨵��������������ַ��*/
            if( connect(client_sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))<0)
              {
                  perror("connected failed");
                  exit(0);
              }
            /* ѭ�����ͽ������ݣ�send�������ݣ�recv�������� */
       //while(1)
        //   {
             //printf("send msg to server: \n");
             //fgets(sendbuf, 1024, stdin);
             printf("start to send message\n", );
             for (int i = 0; i < 100; i++) {
             /* ��������˷������� */
               if( send(client_sockfd, i, strlen(i), 0) < 0)
                {
                    printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
                    exit(0);
                }

             /* ���ܷ������˴����������� */
            //    while(rec_len = recv(client_sockfd,receivebuf, MAXLINE,0)) != -1)
            //     {
            //          perror("recv error");
            //          exit(1);
            //     }
            //  receivebuf[rec_len]='\0';
            //  printf("Response from server: %s\n",receivebuf);

             }
          //  }
       /* �ر��׽��� */
       sleep 10s;
    close(client_sockfd);
    return 0;
        }
