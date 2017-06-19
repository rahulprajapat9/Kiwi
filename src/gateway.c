#include "gateway/modem.h"
#include "gateway/wireless.h"
#include "common/device.h"
#include <string.h>

/**
 * This function is polled by the main loop and should handle any packets coming
 * in over the modem or 868 MHz communication channel.
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
  modem_enqueue_outgoing(ack,32);
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
          1.1 check if data is coming from backend
            1.1.1 check if dev_id is its own id (or belongs to any sensor)
              1.1.1.1 ping
              1.1.1.2 restart_controller
            1.1.2 if it belongs to any other sensor node under it 
              1.1.1.1 relay cmd(1 byte) and token_ki(16 byte) to that sensor node
          1.2 check if data is coming from sensor
            1.1.1 form a payload with initial 16 bytes as sensor's dev_id and data (varibale), after that send it to the backend.
  
  --> I have implemented continuously polling here. Alternatively this polling can be shifted to main function.
  
  --> Also tested by implementing RTOS pthreads in main function.
  
  --> Data format of payload coming from backend: 
      Dev_id (16 bytes) || cmd (1 byte) || Token ki (16 bytes)
      //Maximum of size of payload is 33 bytes for this assessment
      
  --> Data format of payload coming from sensor: 
      Dev_id (16 bytes) || cmd (1 byte) || Token ki (16 bytes)
      //Maximum of size of payload is 33 bytes for this assessment    
      
  */
 
void handle_communication(void)
{
  bool data_in_fromBackend_flag=0;
  bool data_in_fromSensor_flag=0;
  while(true){
  
      /************data from modem (i.e. backend)************/
        //data_in_fromBackend_flag variable returns true (i.e. 1) when modem_dequeue_incoming(..,..) return true (i.e. 1) and GW performs the task given by backend.
    uint8_t const **backend_recv_data;
    size_t* len_backend_recv_data = NULL;
    
    if(modem_dequeue_incoming(backend_recv_data, len_backend_recv_data)==1){
      data_in_fromBackend_flag = 1;
    }
  
        // once we find data is coming from backend, we perform the task.
    if(data_in_fromBackend_flag==1){

      //Data format : Dev_id (16 bytes) || cmd (1 byte) || Token ki (16 bytes)
      //Reading dev_id first.
      device_id_t payload_deviceID = {0};
      memcpy(payload_deviceID.bytes, &(&backend_recv_data)[0], 16* sizeof(uint8_t));
      
      //Check if this dev_id belongs to its own or to any of the sensor node. If it belongs to its own, then perform the task on itself. Else move else part of the following if control which is essentially forwarding this packet to sensor node.
      // Two commands possible on gateway: cmd 1 = ping; cmd 2 = Restart;
      device_id_t GW_own_dev_id = get_device_id();
      
      if(payload_deviceID.bytes == GW_own_dev_id.bytes){
        //read the command instructed by backend (which is placed in 17th byte of the payload)
        //uint8_t cmd = &(&backend_recv_data)[16];
	uint8_t cmd=1;

        switch (cmd)
        {
          case 1: 
            pong();
            break;
          case 2: 
            restart_controller();
            break;
          default: 
            //Anything other than specified commands, do nothing
            break; 
        }
      }
      else{
        //forward the payload to respective sensor and this payload essentially contains : "cmd(1 byte) and token_ki(16 byte)".
        uint8_t data_to_sensor[WIRELESS_PAYLOAD_LENGTH] = {0};
        memcpy(data_to_sensor, &(&backend_recv_data)[16], 17* sizeof(uint8_t));
        wireless_enqueue_outgoing(payload_deviceID,data_to_sensor);
      }
      
      data_in_fromBackend_flag = 0; // setting flag back to 0
    }
    
    /************data from wireless (i.e. sensor)************/
            //data_in_fromSensor_flag variable returns true (i.e. 1) when wireless_dequeue_incoming(..,..) return true (i.e. 1) and GW relays this data to the backend.
    device_id_t *sensor_device_id;
    device_id_t temp; //temp is used just for dereferencing
    sensor_device_id = &temp;
    uint8_t sensor_recv_data[WIRELESS_PAYLOAD_LENGTH] = {0};
      
    if(wireless_dequeue_incoming(sensor_device_id,sensor_recv_data)==1){
      data_in_fromSensor_flag = 1;
    }
    
    if(data_in_fromSensor_flag==1){
        //form a payload with initial 16 bytes as sensor's dev_id and data (varibale), after that send it to the backend.
      uint8_t len_sensor_recv_data = strlen((char *)sensor_recv_data);
      uint8_t sensor_data_to_backend[MODEM_MAX_PAYLOAD_LENGTH] = {0};
      memcpy(&sensor_data_to_backend[0], &(*sensor_device_id).bytes[0], 16* sizeof(uint8_t));
      memcpy(&sensor_data_to_backend[16], &sensor_recv_data[0], len_sensor_recv_data* sizeof(size_t));
      
      //once payload formation is done, I relay this data to modem.
      size_t len_sensor_data_to_backend = strlen((char *)sensor_data_to_backend);
      modem_enqueue_outgoing(sensor_data_to_backend,len_sensor_data_to_backend);  
      
      data_in_fromSensor_flag = 0;  //setting flag back to 0
      
    }
  }
}
