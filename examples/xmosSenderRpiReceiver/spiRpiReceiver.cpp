#include <iostream>
#include <errno.h>
#include <wiringPiSPI.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wiringPi.h>
#include "rpiReceiver.h"
using namespace std;

//this function is executed then an interruption happens
void xmosInterrupt(void)
{
  int len;    //length of the data in bytes
  unsigned char buffer [MAX]; //hold the data
  len = rpiReceive (buffer);  //receive the data
  for (int k = 0; k < len; k++) //print out
    printf(" %c ", int (buffer[k]));
  printf ("|%d\n", eventCounter++); //counts the amount of transfers
}



int main()
{
  int fd, result;              //fd, identifier of the spi, result hold the amount of bytes being transfered
  fd = wiringPiSPISetup(CHANNEL,5000000);  //start the spi bus master
  cout << "Init result: " << fd << endl;   //shows the result
  sleep (1);   //wait a sec
  if (wiringPiSetup() < 0)  //configures the wiring pi
    {
      fprintf(stderr, "unable to setup wiring pi:s\n",strerror(errno));
      return 1;
    }
  if (wiringPiISR (BUTTON_PIN, INT_EDGE_RISING, &xmosInterrupt) < 0) //configures the interruption
    {
      fprintf(stderr, "unable to setup wiring pi ISR:s\n",strerror(errno));
      return 1;
    }
  printf("INIT\n"); //welcome msg
  while (1); //does nothing, the ISR handles everithing
    
  return 0;

}
