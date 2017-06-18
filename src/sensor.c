#include "sensor/wireless.h"
#include "sensor/ki_store.h"
#include "sensor/door.h"
#include "common/device.h"

/**
 * This function is polled by the main loop and should handle any packets coming
 * in over the 868 MHz communication channel.
 */
void handle_communication(void)
{
  //printf("reading payload coming from the backend ... \n");
  uint8_t recv_data[WIRELESS_PAYLOAD_LENGTH];
  wireless_dequeue_incoming(recv_data);
  
  device_id_t payload_deviceID = {0};
  payload_deviceID.bytes = &recv_data[0];
  
  //printf("finished reading device id from payload as bytes and words format \n");
  //printf("Bytes[0] is : %X \n", payload_deviceID.bytes[0]);
  //printf("Words[0] is : %X \n", payload_deviceID.words[0]);
  
  //printf("reading token id from payload \n");
  uint8_t* token_id = &recv_data[16];

}
