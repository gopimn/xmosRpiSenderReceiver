#ifndef RPISENDER_H_
#define RPISENDER_H_
#define MAX 64                 //Maximum size of the data being send. 
#define BUTTON_PIN 0           //Interruption pin that indicates that the xmos
static const int CHANNEL = 1;  //The SS we're using of the RPI is the channel 1.
//This function sends len amount of bytes to the xmos receiver.
int rpiSend(unsigned char msg[MAX],int len )
{
  unsigned char buffer[MAX];             //Message variable.
  int result;                //The amount of bytes transfered are stored here.
  buffer[0]=len;             //First byte is the size =)    
  buffer[1]='\0';            //Close the buffer TODO is this really necesary   
  result = wiringPiSPIDataRW(CHANNEL, buffer, 1); //Transfer the size oftheDATA
  for (int i =0;i< len;i++)  //Fill the buffer with the message DATA.
    buffer[i]=msg[i];        //len bytes 
  buffer[len]='\0';          //Close the buffer TODO is this really necesary
  result = wiringPiSPIDataRW(CHANNEL, buffer, len);//Transfer the DATA 
  return result;            //return the amount of succesfully transfered bytes.
}

#endif /* RPISENDER_H_*/
