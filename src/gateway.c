#include "gateway/modem.h"
#include "gateway/wireless.h"
#include "common/device.h"

/**
 * This function is polled by the main loop and should handle any packets coming
 * in over the modem or 868 MHz communication channel.
 */
// cmd 1
void pong (void){
  uint8_t ack[WIRELESS_PAYLOAD_LENGTH];
  for(uint8_t i = 0; i<32; i++){
    ack[i]=0x01;
  }
  modem_enqueue_outgoing(ack,32);
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
 * in over the modem or 868 MHz communication channel.
 */
void handle_communication(void)
{
  uint8_t downstream=0;
  uint8_t upstream=0;
  //downstream = 1;
  while(true){
  
      uint8_t** backend_recv_data = 0;
      size_t* len_backend_recv_data;
      
      
  
    if(modem_dequeue_incoming(backend_recv_data, len_backend_recv_data)==1){
      downstream = 1;
      printf("modem is 1 \n");
    }
  
    if(downstream==1){
      /*******from backend********/
      printf("reading payload coming from the backend ... \n");
      
      uint8_t** backend_recv_data = 0;
      size_t* len_backend_recv_data;
      
      modem_dequeue_incoming(backend_recv_data, len_backend_recv_data);
      //printf("recv_data is : %X \n", backend_recv_data);
      
      device_id_t payload_deviceID = {0};
      memcpy(payload_deviceID.bytes, &(&backend_recv_data)[1], 16* sizeof(uint8_t));
      
      printf("finished reading device id from payload as bytes and words format \n");
      printf("Bytes[0] is : %X \n", payload_deviceID.bytes[0]);
      printf("Words[0] is : %X \n", payload_deviceID.words[0]);
      
      device_id_t GW_own_dev_id = get_device_id();
      if(payload_deviceID.bytes == GW_own_dev_id.bytes){
        printf("my own dev \n");
        
        //uint8_t cmd = &(&backend_recv_data)[16];
        //printf("cmd is : %d \n", cmd);
        
        uint8_t* tt = *backend_recv_data[16];
        uint8_t cmd = *tt;
        
        cmd = 1;
        switch (cmd)
        {
          case 1: 
            printf("Pong() is requested by backend \n");
            pong();
            break;
          case 2: 
            printf("Restart_GW is requested by backend \n");
            restart_controller();
            break;
          default: 
            printf("Anything other than specified commands, do nothing \n");
            break; 
        }
      }
      else{
        printf("it's sensor's dev id \n");
        
        uint8_t data_to_sensor[WIRELESS_PAYLOAD_LENGTH] = {0};
        
        memcpy(data_to_sensor, &(&backend_recv_data)[16], 17* sizeof(uint8_t));
        wireless_enqueue_outgoing(payload_deviceID,data_to_sensor);
      }
      
      //printf("reading token id from payload \n");
      //uint8_t* token_id = &recv_data[16];    
      
      downstream = 0;
      
    }
    
    device_id_t *sensor_device_id = 0;
      uint8_t sensor_recv_data[WIRELESS_PAYLOAD_LENGTH] = {0};
      
    if(wireless_dequeue_incoming(sensor_device_id,sensor_recv_data)==1){
      upstream = 1;
      printf("sensor is 1 \n");
    }
    
    if(upstream==1){
      /************from sensor************/
      device_id_t *sensor_device_id = 0;
      uint8_t sensor_recv_data[WIRELESS_PAYLOAD_LENGTH] = {0};
      wireless_dequeue_incoming(sensor_device_id,sensor_recv_data);
      
      size_t len_sensor_recv_data = strlen(sensor_recv_data);
      printf("len_sensor_recv_data is : %d \n", len_sensor_recv_data);
      modem_enqueue_outgoing(sensor_recv_data,len_sensor_recv_data);  
      
      upstream = 0;
      
    }
  
  }

  
}