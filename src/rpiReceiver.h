#ifndef RPIRECEIVER_H_
#define RPIRECEIVER_H_

//using the channel 1 of the rpi
static const int CHANNEL = 1;
//this is the interruption receiver
#define BUTTON_PIN 0
//max amount of bytes
#define MAX 64

//counts the amount of transfers
volatile int eventCounter = 0;

//Receives first, the size of the data, after the data
int rpiReceive (unsigned char msg[MAX])
{
  int result;                    //holds the aomunt of bytes transfered 
  unsigned char buffer [MAX];    //receives the data
  buffer[0]=0x0;                 //clean the first byte, we receive the size here
  buffer[1]='\0';                //close 
  result = wiringPiSPIDataRW(CHANNEL, buffer, 1); //transfer size
       int size = int (buffer[0]);   //save the size
       result = wiringPiSPIDataRW(CHANNEL, buffer, size); //transfer data 
  for (int i=0; i<size;i++)
    msg[i]=buffer[i];  //fills the message variable
  return size;  //return the size
}


#endif /*  RPIRECEIVER_H_ */
