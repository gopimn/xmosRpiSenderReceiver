# xmosRpiSenderReceiver
Implements routines to easily communicate the xmos with the Rpi using the xmos spi libraries and the wiringPi spi libraries.<br />

This routines use the wiring:<br />
XMOS1F <--- RPI SCLK (23)<br />
XMOS1A <--- RPI SS ()<br />
XMOS1H ---> RPI MISO ()<br />
XMOS1G <--- RPI MOSI ()<br />
XMOS1D ---> RPI WIRINGPI 1 (11)<br />

As we are using the wiringPi spi library to communicate with the SBC, we can only use it as a master, well, we have attached an interrupt in order to
be able to indicate the master to receive data.<br />

Remember to include the xmos header on the src folder, and to activate the spi library and the debug_printf library on your makefile.w

Remember to include the rpi header file on your folder when using g++. and include the lwiringPi flags when compiling it.<br />

TODO__
1) we must compact the two headers in one, the sender and receiver.
2) on the xmos, spisender should be a general state machine.
