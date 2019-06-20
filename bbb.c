#include <stdint.h>
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include <sys/ioctl.h>

int main()
{
      int fd, count_r,count_t,i;
      unsigned char buff[27];  // the reading & writing buffer
      unsigned int Temperature[2],CO2[2],TVOC[2],Humidity[2],Illuminance[3];
      unsigned int co2,tvoc;
      double tem,hum,illu;
      char sendbuf[4096];
      struct termios opt;       //uart  confige structure
      if ((fd = open("/dev/ttyO1", O_RDWR)) < 0)
      {
          perror("UART: Failed to open the UART device:ttyO1.\n");
          return -1;
      }
      tcgetattr(fd, &opt); // get the configuration of the UART
      // config UART
      opt.c_cflag = B115200 | CS8 | CREAD | CLOCAL;
      // 9600 baud, 8-bit, enable receiver, no modem control lines
      opt.c_iflag = IGNPAR | ICRNL;
      // ignore partity errors, CR -> newline
      opt.c_iflag &= ~(INLCR | ICRNL); //不要回车和换行转换
      opt.c_iflag &= ~(IXON | IXOFF | IXANY);
      opt.c_oflag &= ~OPOST;
      //turn off software stream control
      opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
      //关闭回显功能,关闭经典输入 改用原始输入
      tcflush(fd,TCIOFLUSH);        // 清理输入输出缓冲区
      tcsetattr(fd, TCSANOW, &opt); // changes occur immmediately
      for(i = 0;i < 100;i++)
      {
          if ((count_r = read(fd,buff,27))<0)
              perror("ERR:No data is ready to be read\n");
          else if (count_r == 0)
              printf("ERR:No data is ready to be read\n");
          else
          {
            CO2[0]=(unsigned int)(buff[6]*256);
            CO2[1]=(unsigned int)(buff[7]);
            co2=CO2[0]+CO2[1];
            printf("co2:%d\n",co2);
            //sprintf(sendbuf,"%.3lf",tvoc);
            TVOC[0]=(unsigned int)(buff[8]*256);
            TVOC[1]=(unsigned int)(buff[9]);
            tvoc=TVOC[0]+TVOC[1];
            printf("tvoc:%d\n",tvoc);
            Temperature[0]=(unsigned int)(buff[14]*256);
            Temperature[1]=(unsigned int)(buff[15]);
            tem=(double)(Temperature[0]+Temperature[1]);
            tem=tem/65536*175.72-46.85;
            printf("tem:%.3lf\n",tem);
            Humidity[0]=(unsigned int)(buff[16]*256);
            Humidity[1]=(unsigned int)(buff[17]);
            hum=(double)(Humidity[0]+Humidity[1]);
            hum=hum/65536*125-6;
            printf("hum:%.3lf%%\n",hum);
            //Illuminance[0]=(unsigned int)((buff[18]/16)*4096;
            //Illuminance[1]=(unsigned int)(buff[18]);
            //illu=(double)(Illuminance[0]+Illuminance[1]);
            //illu=illu;
            //printf("illu:%.3lf\n",illu);
          }
      }
      close(fd);
      return 0;
}
