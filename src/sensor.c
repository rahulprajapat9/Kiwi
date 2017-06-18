#include "sensor/wireless.h"
#include "sensor/ki_store.h"
#include "sensor/door.h"
#include "common/device.h"
#include <stdio.h>
#include <string.h>

// cmd 1
void pong (void){
  uint8_t ack[WIRELESS_PAYLOAD_LENGTH];
  for(uint8_t i = 0; i<32; i++){
    ack[i]=0x01;
  }
  wireless_enqueue_outgoing(ack);
  printf("Pong sent \n");
}

// cmd 2
void restart_controller (void)
{
  //((void (far *) (void)) 0x000000) ();
  // Requires external library. Is it allowed in Kiwi firmware?
  printf("Node_restart initialized \n");
}


/**
 * This function is polled by the main loop and should handle any packets coming
 * in over the 868 MHz communication channel.
 */
void handle_communication(void)
{
  printf("reading payload coming from the backend ... \n");
  uint8_t recv_data[WIRELESS_PAYLOAD_LENGTH];
  wireless_dequeue_incoming(recv_data);
  
  device_id_t payload_deviceID = {0};
  memcpy(payload_deviceID.bytes, &recv_data[0], 16* sizeof(uint8_t));
  
  printf("finished reading device id from payload as bytes and words format \n");
  printf("Bytes[0] is : %X \n", payload_deviceID.bytes[0]);
  printf("Words[0] is : %X \n", payload_deviceID.words[0]);
  
  printf("reading token id from payload \n");
  uint8_t* token_id = &recv_data[16];

  printf("five cases corresponding to five commands \n");
  uint8_t cmd = 0;
  switch (cmd)
  {
    case 1: 
      printf("Pong() is requested by backend \n");
      pong();
      break;
    case 2: 
      printf("Restart_node is requested by backend \n");
      restart_controller();
      break;
    case 3: 
      printf("Adding a new token to the ki store \n");
      ki_store_add(token_id);
      break;
    case 4: 
      printf("Removing specified token from the ki store \n");
      ki_store_remove(token_id);
      break;
    case 5: 
      printf("Actuating command \n");
      door_trigger();
      break;
    default: 
      printf("Anything other than specified commands, do nothing \n");
      break; 
  }
}