#include "protocolUtils.h"
#include "usart.h"
#include "string.h"
uint8_t cmdByte[32] = {0};


void addProtocolCmd(Axis3_Float_Typedef* angle){
    int16_t tmpData;
    uint8_t checkSum=0;
    uint8_t i;
  //protocol head
    cmdByte[0] = 0x88;
    cmdByte[1] = 0xAF;
    cmdByte[2] = 0x1C;

    tmpData = (int16_t) angle->x * 100;
    memcpy(cmdByte+21, &tmpdata, 2);

    tmpData = (int16_t) angle->y * 100;
    memcpy(cmdByte+23, &tmpdata, 2);

    tmpData = (int16_t) angle->z * 10;
    memcpy(cmdByte+25, &tmpdata, 2);

    for(i=0;i<31;i++){
      checkSum+=cmdByte[i];
    }
    cmdByte[31] = checkSum;
}

void sendDataProtocol(Axis3_Float_Typedef* angle){
  uint8_t i;
  addProtocolCmd(angle);
  for(i=0;i<32;i++){
    USART_SendChar(USART1,cmdByte);
  }
}
