#include <xs1.h>            //Use of XS1 type declarations.
#include <spi.h>            //SPI xmos library.
#include <debug_print.h>    //Use of debug_printf. deactivate flag on debug_print.h to disable messages via the console
#include "xmosSender.h"    //RPI to XMOS SPI sending routines.


in  port                spiClock         = XS1_PORT_1F; //Clock of the spi bus.
in  port                spiSelect        = XS1_PORT_1A; //SS of the spi bus, there's just one client on this example.
out buffered port:32    spiMISO          = XS1_PORT_1H; //MISO pin of the spi bus.
in  buffered port:32    spiMOSI          = XS1_PORT_1G;  //MOSI pin of the spi bus.
out port                interruptPort    = XS1_PORT_1D;  //This port indicates the master spi to retrieve data.
clock                   clkInterrupt     = XS1_CLKBLK_1; //Clock that times the output of the interruption
clock                   clkb0            = XS1_CLKBLK_3; //Clock block for the slave spi interface.

//This example task sends HOLA each half second to the master.
void dataSupplierTask(client interface senderSpi_if cc)
{
   debug_printf("INIT: dataSuplierTask\n");      //welcome msg
   int amount;                                   //amount of bytes to transfer
   while(1)
   {
       unsigned int dataMsg[MAX] = {'H','O','L','A'}; //message, 4 bytes
       amount =4;                                     //length of the message
       cc.xmosSend(dataMsg,amount);                   //Send data using the routine
       delay_milliseconds(500);                       //each half second
   }
}


int main(void) {
  debug_printf("INIT GLabz_0.xc\n");                          //hello message
  configure_clock_rate(clkInterrupt,100,40);                  //configures the output clock speed to 100/40 Mhz
  configure_out_port(interruptPort, clkInterrupt,1);          //attach the clock to the output
  start_clock(clkInterrupt);                                  //start the clock
  interface spi_slave_callback_if i_spi;                      //xmos library spi slave interface.
  interface senderSpi_if ccc;                                 //spi sender interface
  par {
        spi_slave(i_spi, spiClock, spiMOSI, spiMISO, spiSelect, clkb0, SPI_MODE_1,SPI_TRANSFER_SIZE_8); //start the spi slave
        spiSender(i_spi,ccc,interruptPort);                                                             //start the sender
        dataSupplierTask(ccc);                                                                          //uses the routiune.
      }
  return 0;
}
