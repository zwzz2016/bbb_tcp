#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#include "mysql.h"

#define MAXLINE    4096
#define SERV_PORT  9997    /*接收数据端口号（客户端）*/

int main(int argc, char** argv)
{
    time_t timep;
    int    len,res;
    int    socket_fd, connect_fd,client_fd;
    char   ipv4[13],temperator[6],humidity[6],illuminance[6],co2[6],tvoc[6],wap[6];/*定义所有传感器数组*/
    struct sockaddr_in servaddr;     /* 服务器端网络地址结构体 */
    char    buf[MAXLINE],cmd[1024]={0};

    /*创建服务器端套接字--IPv4协议，面向连接通信，TCP协议*/
    if( (socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
    {
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

     /*建立数据库连接*/
     MYSQL *conn_ptr;
     conn_ptr = mysql_init(NULL);
     if(!conn_ptr)
     {
       fprintf(stderr,"mysql_init failed\n");
       return EXIT_FAILURE;
     }

     /*数据库连接参数设置*/
     conn_ptr = mysql_real_connect(conn_ptr,"localhost","root","123456","sensor_data",0,NULL,0);

     if(conn_ptr)
       printf("Connection success\n");
     else
       printf("Connection failed\n");

     /*接受客户端传过来的数据*/
     while((len = recv(connect_fd, buf, MAXLINE, 0))>0)
     {
       //buf[len] = '\0';

       /*分割传输数据*/
       for(int i=0,j=0;i<=12;i++)
      {
        ipv4[j]=buf[i];
        j++;
      }
      ipv4[13]='\0';
      for(int i=13,j=0;i<=18;i++)
      {
        temperator[j]=buf[i];
        j++;
      }
      temperator[6]='\0';
      for(int i=19,j=0;i<=24;i++)
      {
        humidity[j]=buf[i];
        j++;
      }
      humidity[6]='\0';
      for(int i=25,j=0;i<=30;i++)
      {
        illuminance[j]=buf[i];
        j++;
      }
      illuminance[6]='\0';
      for(int i=31,j=0;i<=36;i++)
      {
        co2[j]=buf[i];
        j++;
      }
      co2[6]='\0';
      for(int i=37,j=0;i<=42;i++)
      {
        tvoc[j]=buf[i];
        j++;
      }
      tvoc[6]='\0';
      for(int i=43,j=0;i<=48;i++)
      {
        wap[j]=buf[i];
        j++;
      }
      wap[6]='\0';

      /*获取系统时间*/
      time (&timep);
      printf("%s",ctime(&timep));

      /*生成数据库语句*/
      sprintf(cmd, "insert into sensor_data(time,ip,temperator,humidity,illuminance,co2,tvoc,wap) values('%s','%s','%s','%s','%s',$
      res = mysql_query(conn_ptr, cmd);

      if(!res)  /*ret != 0 表示出错 */
      {
        printf("Inserted by %lu rows\n",(unsigned long)mysql_affected_rows(conn_ptr));
      }
      else  /*打印出错及相关信息*/
      {
        fprintf(stderr, "Insert error %d:%s\n", mysql_errno(conn_ptr), mysql_error(conn_ptr));
      }

      //printf("receive message from client: %s\n", buf);
      /*向客户端发送回应数据*/
      //printf("send message to client: \n");
      //fgets(sendbuf, 4096, stdin);
      // if( send(client_fd, buf, strlen(buf), 0) < 0)
      //   {
      //      printf("send messaeg error: %s(errno: %d)\n", strerror(errno), errno);
      //      exit(0);
      //   }
    }
 close(connect_fd);
 close(socket_fd);
 mysql_close(conn_ptr);
 return 0;
}
