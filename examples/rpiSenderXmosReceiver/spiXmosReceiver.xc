#include <xs1.h>            //Use of XS1 type declarations.
#include <spi.h>            //SPI xmos library.
#include <debug_print.h>    //Use of debug_printf. deactivate flag on debug_print.h to disable messages via the console
#include "xmosReceiver.h"    //RPI to XMOS SPI sending routines.

in  port                spiClock         = XS1_PORT_1F; //Clock of the spi bus.
in  port                spiSelect        = XS1_PORT_1A; //SS of the spi bus, there's just one client on this example.
out buffered port:32    spiMISO          = XS1_PORT_1H; //MISO pin of the spi bus.
in  buffered port:32    spiMOSI          = XS1_PORT_1G; //MOSI pin of the spi bus.
clock                   clkb0            = XS1_CLKBLK_1;//Clock block for the slave spi interface.

//This task waits for the data ready signal of the receiverSpi_if interface,
//and prints the data on the screen.
void datareceiverTask(client interface receiverSpi_if cc)
{
   unsigned int  values[MAX]={'\0'};   //Array that gets full of DATA from the RPI.
   int amount=0;                     //Amount of times the DATA has being received.
   int len;                                               //The length of the DATA.
   debug_printf("datareceiverTask:\t\tINIT!\n");               //Task INIT message.
   while(1)                                               //Main while of the task.
   {
       select                        //We're using a select to wait for data_ready.
       {
       case cc.data_ready():             //if data is ready,
           len=cc.xmosReceive(values);   //retrieve the data and save it on values, len will hold the length of the data in bytes.
           debug_printf("datareceiverTask:\t\tvalue: ");          //Debug task msg.
           for(int i=0;i<len;i++)                              //Printing the DATA.
               debug_printf("%x",values[i]);
           debug_printf("|%d\n",amount++);             //Show the amount of times we've recieved DATA.
           break;
       }//select
   }//main while
}//void datareceiverTask

//Main
int main(void) {
  debug_printf("INIT GLabz_01_xmosReceiverSpi.xc\n");                    //Welcome msg.
  interface spi_slave_callback_if i_spi;                  //xmos spi library interface.
  interface receiverSpi_if ccc;                    //RPI to XMOS SPI sending interface.
  debug_printf("________________________________\n");                    //end of init.
  par
  {
        spi_slave(i_spi, spiClock, spiMOSI, spiMISO, spiSelect, clkb0, SPI_MODE_1,SPI_TRANSFER_SIZE_8);  //spi xmos library slave task.
        spiSender(i_spi,ccc);                                                                           //RPI to XMOS SPI sending task.
        datareceiverTask(ccc);                                                           //User of the RPI to XMOS SPI sending routine.
  }//par
  return 0;
}//main


