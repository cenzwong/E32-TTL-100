#include "E32.h"
//#include "config.h"

E32 myE32; // to be config in config.h
//target device address and Channel
E32Device deviceA = {0xAAAA, 0x17};
E32Device deviceB = {0xBBBB, 0x17};

uint8_t dataA[] = {0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0x10,\
                     0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x20,\
                     0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0xAA, 0x1E, 0x1F, 0x20};

uint8_t dataB[] = {0xB1, 0xB2, 0xB3, 0xB4, 0xB6, 0xB7, 0xB8, 0xB9, 0x10,\
                    0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x20,\
                    0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0xBB };

uint8_t dataC[] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC};

uint8_t dataD[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50};
E32Data FrameD = {dataD, sizeof(dataD)};

E32Data FrameA = {dataA ,sizeof(dataA)};
E32Data FrameB = {dataB ,sizeof(dataB)};
E32Data FrameC = {dataC, sizeof(dataC)};
E32Data inFrame = {};

void setup() {
  // put your setup code here, to run once:s
  Serial.begin(9600);
  Serial.write(0xFF);
  myE32.begin();
}

void loop() {
    if(myE32.receiveComplete()){
      Serial.write(0xac);
      inFrame = myE32.getMessage();
      
//DEBUG
      for(int i = 0; i < inFrame.dataLen ; i++){
        Serial.write(inFrame.dataPtr[i]);       
      }
//endDEBUG

    //return the data back to the deviceA
    while(!myE32.isAUXHigh());  //wait until it becmoe high
    myE32.changeMode(Mode_Normal);
    myE32.sendMessage(deviceA, inFrame);
    myE32.changeMode(Mode_Power_Saving);

    }
}

