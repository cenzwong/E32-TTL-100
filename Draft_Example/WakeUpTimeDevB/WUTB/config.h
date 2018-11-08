//==== pin assignment====
#define E32_M0  8
#define E32_M1  9
#define E32_RXD 10
#define E32_TXD 11
#define E32_AUX 12
SoftwareSerial E32Serial(E32_TXD, E32_RXD);  // RX, TX swap intensionally
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
#define Para_Setting_TransPower         TPdbm10


