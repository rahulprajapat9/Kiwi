bool wireless_dequeue_incoming(uint8_t data[static WIRELESS_PAYLOAD_LENGTH])
{
  for(uint8_t i = 0; i<16; i++){
    data[i]=0x0F;
  }
  for(uint8_t i = 16; i<32; i++){
    data[i]=0xEE;
  }
  return true;
}