#include <stdlib.h>
#include <stdio.h>
//#include <string>
#include <time.h>

#include "mysql.h"

int main(int argc,char *argv[])
{
  time_t timep;
  int res;
  MYSQL *conn_ptr;
  conn_ptr = mysql_init(NULL);
  char ipv4[14],temperator[7],humidity[7],illuminance[7],co2[7],tvoc[7],wap[7];/*定义所有传感器数组*/
  char buf[]="192.168.10.01t:11.1h:22.1i:33.1c:44.1v:55.1w:66.1";
  char cmd[1024]={0};
//  std::string ipv4,temperator,humidity,illuminance,co2,tvoc,wap;

  if(!conn_ptr)
  {
    fprintf(stderr,"mysql_init failed\n");
    return EXIT_FAILURE;
  }

  conn_ptr = mysql_real_connect(conn_ptr,"localhost","root","123456","sensor_data",0,NULL,0);

  if(conn_ptr)
    printf("Connection success\n");
  else
    printf("Connection failed\n");

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
   sprintf(cmd, "insert into sensor_data(time,ip,temperator,humidity,illuminance,co2,tvoc,wap) values('%s','%s','%s','%s','%s','%s'$
   res = mysql_query(conn_ptr, cmd);

   if(!res)  /*ret != 0 表示出错 */
   {
     printf("Inserted by %lu rows\n",(unsigned long)mysql_affected_rows(conn_ptr));
   }
   else  /*打印出错及相关信息*/
   {
     fprintf(stderr, "Insert error %d:%s\n", mysql_errno(conn_ptr), mysql_error(conn_ptr));
   }

  mysql_close(conn_ptr);

  return EXIT_SUCCESS;
}

