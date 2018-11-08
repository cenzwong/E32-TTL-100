#include "Arduino.h"
#include "HardwareSerial.h"
#include <SoftwareSerial.h>

typedef struct {
  uint16_t addr;
  uint8_t channel;  
}E32Device;
#define Boradcast_addr    0xFFFF

//=======Operation_Mode_Para=====
#define Mode_Normal         0
#define Mode_Wake_Up        1
#define Mode_Power_Saving   2
#define Mode_Sleep          3
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

#define Para_setting_Channel_reg  (Para_Setting_ChannelinM-410)

//bit/byteShifting Const
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

typedef struct{
    uint8_t * dataPtr;
    uint8_t dataLen;
  } E32Data;

class E32{
  public:
    E32();
    void begin(void);
    void changeMode(uint8_t mode);
    void setConfig(void);
    void returnConfig(void);
    void sendMessage(E32Device targetDevice, E32Data outData);
    bool receiveComplete(void);
    E32Data getMessage();
  private:
      E32Data inDataAry;
      int8_t inDataTop;
};

