bool modem_dequeue_incoming(uint8_t **data, size_t *length){
  size_t t = MODEM_MAX_PAYLOAD_LENGTH;
  length = &t;
  
  uint8_t temp;
  uint8_t** te;
  
  temp = 0x0F; // dev id
  for(uint8_t i = 0; i<16; i++){
    te= data+i;
    te=&temp;
  }
  
  temp = 1; // cmd (ping)
  te = data+16;
  te = &temp;
  
  temp = 0xEE; // token id
  for(uint8_t i = 17; i<33; i++){
    te= data+i;
    te=&temp;
  }
  
  return 0;
}