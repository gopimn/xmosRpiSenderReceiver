#ifndef XMOSRECEIVER_H_
#define XMOSRECEIVER_H_


#define MAX 64  //The maximum size of the data array

//States of the spi receiver task.
enum receiver_state_t
{
  IDLER = 0,
  RECVSIZE = 1,
  RECVDATA = 2
};

//This is the interface that implements the receiver spi routines
interface receiverSpi_if
{
//Recieve data on the variable values,
[[clears_notification]][[guarded]]int xmosReceive(unsigned int (& values)[MAX]);

//Signal the client that the data is ready to send.
[[notification]]
 slave void data_ready(void);
};


//This is the task that controls the spi slave interface.
void spiSender( server spi_slave_callback_if i_spi,server interface receiverSpi_if cc)
{
   enum receiver_state_t state_recv = RECVSIZE; // the state of the machine that receives
   int actual=0;                                //actual byte of data received
   int arrayRecv[MAX];                          //This is the variable that holds the data
   int lenMsgRecv=0;                            //Amount of bytes of data
   while (1) {
    select
      {
    case i_spi.master_ends_transaction(void):   //Not used
      break;
    case i_spi.master_requires_data(void) -> uint32_t data: //Not used
            data=0x0;
            break;
    case i_spi.master_supplied_data(uint32_t datum, uint32_t valid_bits): //The master delivers data
            switch (state_recv)  //State machine of the receiver
                   {
                    case RECVSIZE:              // we have received the size of the data
                      lenMsgRecv=datum;         //DATUM!
                      state_recv = RECVDATA;    //change the state
                      actual=0;                 //initializes the byte counter
                     break;

                    case RECVDATA:                   // we are receiving the actual data
                        if (actual<(lenMsgRecv-1))   // Wait for the last one
                        {
                            arrayRecv[actual++]=datum; //save the byte
                            state_recv = RECVDATA;   //still on the recvdata state
                        }
                        else                        //LAST ONE
                        {
                            arrayRecv[actual++]=datum; //save the last byte
                            state_recv = RECVSIZE;  //change the state
                            cc.data_ready();      //notice the slave task that we have received data
                        }
                     break;
                     }
            break;

     case cc.xmosReceive(unsigned int (& values)[MAX])-> int len: //when the slave task requires data
           for(int i=0;i<lenMsgRecv;i++)        //fill the array with the data
              values[i]=arrayRecv[i];
           values[lenMsgRecv]='\0';            //close the array
           len = lenMsgRecv;                //return the amount of bytes
         break;
     }
  }
}


#endif /* XMOSRECEIVER_H_ */
