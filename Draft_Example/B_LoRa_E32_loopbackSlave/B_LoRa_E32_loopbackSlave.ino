//This is the B program: COM2

#include <SoftwareSerial.h>

#define DEBUG

//==== pin assignment====
#define E32_M0  8
#define E32_M1  9
#define E32_RXD 10
#define E32_TXD 11
#define E32_AUX 12

//=======Operation_Mode_Para=====
#define Mode_Normal         0
#define Mode_Wake_Up        1
#define Mode_Power_Saving   2
#define Mode_Sleep          3
//the Sleep mode can be use as config

//Para_Setting_CMD
#define Para_Save 0xC0
#define Para_One  0xC2

//Para_Setting_UART_Par_bit
#define pbit_8N1 0b00
#define pbit_8O1 0b01
#define pbit_8E1 0b10
//Baudrate
#define brate_1200 0b000
#define brate_2400 0b001
#define brate_4800 0b010
#define brate_9600 0b011
#define brate_19200 0b100
#define brate_38400 0b101
#define brate_57600 0b110
#define brate_115200 0b111
//Air data rate
#define adrate_0k3 0b000
#define adrate_1k2 0b001
#define adrate_2k4 0b010
#define adrate_4k8 0b011
#define adrate_9k6 0b100
#define adrate_19k2 0b101

//==para for the option
enum TransEnBit {Transparent, Fixed};
enum IODriverMode {open_collector, push_pull};
enum WirelessWakeUpTime {
  WUTms250, WUTms500, WUTms750, WUTms1000, WUTms1250, WUTms1500, WUTms1750, WUTms2000
};
enum FECSwitch {FECoff, FECon};
enum TransPower {TPdbm20, TPdbm17, TPdbm14, TPdbm10};

//BYTE shifting
#define HEAD 0
#define ADDH 1
#define ADDL 2
#define SPED 3
#define CHAN 4
#define OPTION 5
//SPED_USART_bit shift
#define SPED_USART_pbit 6
#define SPED_USART_brate 3
#define SPED_USART_adrate 0
//OPTION bit shift
#define OPT_Mode 7
#define OPT_IOMode 6
#define OPT_Waketime 3
#define OPT_FEC_switch 2
#define OPT_Transmission_power 0

#define Boradcast_addr    0xFFFF
typedef struct {
  uint16_t addr;
  uint8_t channel;  
}Device;

//===============********User Define*************==================
#define Operation_Mode            Mode_Normal
#define Para_Setting_CMD          Para_Save
#define Para_Setting_ADDR         0xBBBB
#define Para_Setting_UART_pbit    pbit_8N1
#define Para_Setting_UART_brate   brate_9600
#define Para_Setting_UART_adrate  adrate_1k2                     
#define Para_Setting_ChannelinM   433   
#define Para_Setting_TranEnbit          Fixed
#define Para_Setting_IODriverMode       push_pull
#define Para_Setting_WirelessWakeUpTime WUTms2000
#define Para_Setting_FECSwitch          FECon
#define Para_Setting_TransPower         TPdbm20        
/*
you own device should match with the above setting
Device device_name(device_address, device_channel)
*/
Device deviceA = {0xAAAA, 0x17};
Device deviceB = {0xBBBB, 0x17};
//======reg_para_set======
#define Para_setting_Channel_reg  (Para_Setting_ChannelinM-410)


void E32_ChangeMode(int mode);
void config_return_saved();
void E32_set_config();
void E32_sendmessage(uint16_t target_addr,uint8_t target_channel, uint8_t* data, uint8_t datalen);
void mainPorgram(char CMD);

struct Data{
  bool isFirstByte;
  uint8_t Datalength;
  uint8_t DataAry[255];
  int8_t top;
  } inData;

SoftwareSerial E32Serial(E32_TXD, E32_RXD); // RX, TX swap intensionally

//========main=============
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  E32Serial.begin(9600);
  pinMode(E32_M0, OUTPUT);
  pinMode(E32_M1, OUTPUT);
  pinMode(E32_AUX, INPUT);
#ifdef DEBUG
  Serial.write(255);
  Serial.write(0);
#endif
  //wait until power on self-check complete
  while(!digitalRead(E32_AUX)); //This wait until it become 
  E32_ChangeMode(Mode_Sleep);
  E32_set_config();
  E32_ChangeMode(Mode_Normal);
  
}

void loop() {
#ifdef DEBUG
  if(Serial.available() > 0){
    char inChar = Serial.read();
    //E32Serial.write(inChar);
    //Serial.write(inChar);

    for(uint8_t i = 0; i < inData.Datalength; i++){
      Serial.write(inData.DataAry[i]);
      }
    
    }
#endif



//only 5 bytesize data is receive
    if(E32Serial.available() > 0){
      char inChar = E32Serial.read();
      if(inData.top <= 0){
          inData.top = inChar;
          inData.Datalength = inChar;
        }else{
          inData.DataAry[inData.Datalength - inData.top] = inChar;
          inData.top--;
          if(inData.top == 0){
            static int counter = 0;
            Serial.print(counter++);
            if(counter == 99){counter = 0;}
            Serial.print(":\tMessage is received successfully\r\n");
            E32_sendmessage(deviceA.addr, deviceA.channel, inData.DataAry, inData.Datalength);
            Serial.println("Sending Back to the master...\r\n");
            }
          }
    }
}

void mainPorgram(char CMD){
  uint8_t dataA[] = {0xAA, 0xAB, 0xAC, 0xAD, 0xAE};
  uint8_t dataB[] = {0xBA, 0xBB, 0xBC, 0xBD, 0xBE};

  
  switch(CMD){

    case 'a':

    E32_sendmessage(deviceA.addr, deviceA.channel, dataA, sizeof(dataA));
    break; 

    case 'b':

    E32_sendmessage(Boradcast_addr, deviceA.channel, dataB, sizeof(dataB));
    break;

    case '\0':
    break;
    
    default:
    break;
    }
}

void E32_ChangeMode(int mode){
  //Mode 0: M1:0; M0:0
  //Mode 1: M1:0; M0:1
  //Mode 2: M1:1; M0:0
  //Mode 3: M1:1; M0:1
  /*
  if(mode & 0b01){digitalWrite(E32_M0,HIGH);Serial.print("I am here");}
  else{ digitalWrite(E32_M0,LOW); }

  if(mode & 0b10){digitalWrite(E32_M1,HIGH);}
  else{ digitalWrite(E32_M1,LOW); }
  */
  digitalWrite(E32_M0,(mode & 0b01));
  digitalWrite(E32_M1,(mode & 0b10));
  while(!digitalRead(E32_AUX)); //This wait until it become high
}

void config_return_saved(){
// C1 C1 C1
    E32Serial.write(193);
    E32Serial.write(193);
    E32Serial.write(193);
    while(!digitalRead(E32_AUX)); //This wait until it become high
}

void E32_set_config(){
  E32Serial.write(Para_Setting_CMD);
  E32Serial.write((Para_Setting_ADDR & 0xFF00) >> 2*4);
  E32Serial.write(Para_Setting_ADDR & 0x00FF);

  E32Serial.write( (Para_Setting_UART_pbit << SPED_USART_pbit) |\
  (Para_Setting_UART_brate << SPED_USART_brate) |\
  (Para_Setting_UART_adrate << SPED_USART_adrate));

  E32Serial.write(Para_setting_Channel_reg);
  
  E32Serial.write(Para_Setting_TranEnbit << OPT_Mode |\
  Para_Setting_IODriverMode << OPT_IOMode |\
  Para_Setting_WirelessWakeUpTime << OPT_Waketime |\
  Para_Setting_FECSwitch << OPT_FEC_switch |\
  Para_Setting_TransPower << OPT_Transmission_power);
  while(!digitalRead(E32_AUX)); //This wait until it become high
  while(E32Serial.available()){
    Serial.write(E32Serial.read());
    }
}
 
void E32_sendmessage(uint16_t target_addr,uint8_t target_channel,uint8_t* data, uint8_t datalen){
  //data format data length
  //[datalen data data data data]
#ifdef DEBUG
//  Serial.write(target_addr >> 8);
//  Serial.write(target_addr);
//  Serial.write(target_channel);  
//  for(uint8_t i = 0; i < datalen; i++)
//    Serial.write(data[i]);
#endif

  E32Serial.write(target_addr >> 8);
  E32Serial.write(target_addr);
  E32Serial.write(target_channel);  
//data start
  E32Serial.write(datalen);
  for(uint8_t i = 0; i < datalen; i++)
    E32Serial.write(data[i]);
  
  while(!digitalRead(E32_AUX)); //This wait until it become high
}


