#include <iostream>
#include <errno.h>          
#include <wiringPiSPI.h>   //spi library from wiringPi
#include <unistd.h>        
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wiringPi.h>
#include "rpiSender.h"    //spi xmos to rpi sender routines
using namespace std;

volatile int eventCounter = 0; //Counts the amount of DATA being send.

int main()
{
  int fd;                   // spi wiring pi identifier
  fd = wiringPiSPISetup(CHANNEL,5000000); //Initializes the spi master
  cout << "Init result: " << fd << endl;  //Shows the initialization result
  sleep (1);                              //Wait 1 sec. 
  if (wiringPiSetup() < 0)                //Configures the wiring Pi   
    {                           //Prints to the error console if needed 
      fprintf(stderr, "unable to setup wiring pi:s\n",strerror(errno));
      return 1;
    }
  printf("INIT\n");          //welcome message
  unsigned char a[MAX]={0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7}; //data test 1
  unsigned char a2[MAX]={0x5,0x6,0x7};                    //data test 2 
  int est;                                        //holds the amount of bytes transfered
  while (1) //this while sends the two data tests with 1.8 secs
  {
    est=rpiSend(a,8);  //send the first one
        printf ("%d %d\n",est,eventCounter++);
        usleep(1800000);
        est=rpiSend(a2,3);//send the second one
        printf ("%d %d\n",est,eventCounter++);
        usleep(1800000);

  }
  return 0;
}
