#include "sensor/wireless.h"
#include "sensor/ki_store.h"
#include "sensor/door.h"
#include "common/device.h"


/**
 * This function is polled by the main loop and should handle any packets coming
 * in over the 868 MHz communication channel.
*/

/* cmd 1 : responding the ping
--> I am considering ack as a 32 byte long value where is byte is 0x01.
--> Assuming backend can implement this in their code.
*/
void pong (void){
  uint8_t ack[WIRELESS_PAYLOAD_LENGTH];
  for(uint8_t i = 0; i<32; i++){
    ack[i]=0x01;
  }
  wireless_enqueue_outgoing(ack);
}

/* cmd 2: restart_controller; 
 --> soft reset the controller by setting program counter to 0 OR hard reset the controller by making controller detect software anomalies through watchdog timer and reset the processor
--> In the both the methods I mentioned above requires either an external library call or requires hardware initialization.
*/
void restart_controller (void)
{
  //((void (far *) (void)) 0x000000) ();
  // Requires external library. Is it allowed in Kiwi firmware?
}

  /*
  --> control flow of handle_communication() function;
        1. continous polling 
          1.1 check if data is coming from gateway
            1.1.1 if data is coming perform the task
              1.1.1.1 ping
              1.1.1.2 restart_controller
              1.1.1.3 add ki_token
              1.1.1.4 remove ki_token
              1.1.1.5 open door
  
  --> I have implemented continuously polling here. Alternatively this polling can be shifted to main function.
  
  --> Also tested by implementing RTOS pthreads in main function.
  */

void handle_communication(void)
{
  bool data_in_fromGW_flag = 0; 
  while(true){
    
    //data_in_fromGW_flag variable returns true (i.e. 1) when wireless_dequeue_incoming(recv_data) return true (i.e. 1) and controller performs the task given by backend.
    uint8_t recv_data[WIRELESS_PAYLOAD_LENGTH];
    if(wireless_dequeue_incoming(recv_data)==1){
      data_in_fromGW_flag = 1;
    }
      
      // once we find data is coming from backend, we perform the task.
    if(data_in_fromGW_flag==1){
      //in 17 byte payload, 1st byte will be cmd; next 16 bytes will be token ki 
      uint8_t cmd = recv_data[0];
      uint8_t* ki_token = &recv_data[1];
    
      //five cases corresponding to five commands
      //cmd 1 = ping; cmd 2 = restart; cmd 3 = add a ki; cmd 4 = remove a ki; cmd 5 = open the door; 
      // all commands will ack to backend except cmd 2 i.e. restart_controller
      ki_store_result_t ki_add_result;
      ki_store_result_t ki_remove_result;
      switch (cmd)
      {
        case 1: 
          pong();
          break;
        case 2: 
          restart_controller();
          break;
        case 3: 
          ki_add_result = ki_store_add(ki_token);
          if(ki_add_result==KI_STORE_SUCCESS){
            pong();
          }
          //else backend will try again after timeout
          break;
        case 4: 
          ki_remove_result = ki_store_remove(ki_token);
          if(ki_remove_result==KI_STORE_SUCCESS){
            pong();
          }
          //else backend will try again after timeout
          break;
        case 5: 
          door_trigger();
          pong();
          break;
        default: 
          //Anything other than specified commands, do nothing
          break; 
      }
      data_in_fromGW_flag = 0;
    }
  }
}
