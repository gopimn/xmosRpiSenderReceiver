#ifndef XMOSSENDER_H_
#define XMOSSENDER_H_

#define MAX 64 //maximum size of the data being transfered

//States of the spi sender task.
enum sender_state_t
{
  IDLE = 0,
  SENDINGSIZE = 1,
  SENDING = 2
};

//this interface implements the sending routine
interface senderSpi_if
{
[[guarded]]void xmosSend(unsigned int (&values)[MAX],int amount);
};


//controls the spi slave interface
void spiSender( server spi_slave_callback_if i_spi,server interface senderSpi_if cc, out port interruptPort)
{
   enum sender_state_t state = SENDINGSIZE; //state of the sender machine
   int actual=0;                            //actual byte being sended
   int i=0;                                 //aux index
   int array[MAX];                          //Holds the data being send
   int lenMsg=16;                           //Length of the message in bytes
   while (1) {
    select
      {
    case i_spi.master_ends_transaction(void):
      actual=0; //This index is used to transmit byte by byte. Must inizialize when transmition ends.
      break;
    case i_spi.master_requires_data(void) -> uint32_t data: //this case sends the data
            switch (state)                                  // to the spi master
                   {
                    case IDLE:                 //IDLE STATE
                       state=IDLE;
                    break;

                    case SENDINGSIZE:         //send size
                      data = lenMsg;
                      state = SENDING;
                     break;
                    case SENDING:             //send the data
                        if (actual<=lenMsg)
                          data = array[actual++];
                        else
                          state = IDLE;
                   break;
                    }
         break;
    case i_spi.master_supplied_data(uint32_t datum, uint32_t valid_bits): //not used
       break;
    case cc.xmosSend(unsigned int (&values)[MAX],int amount): //tells the spi slave to interrupt the master when theres data
               state=SENDINGSIZE;                    // change the sender state
               lenMsg= amount;                      //amount of bytes
               for(i=0;i<amount;i++)
               {
                  array[i]=values[i];               //fill the data array
               }
               interruptPort<:1;                    //interrupt the RPI
               interruptPort<:0;                    //OSOM!
               break;
     }
  }
}



#endif /* XMOSSENDER_H_ */
