

#include <SPI.h>
#include "mcp2518fd_can.h"
#include "mcp2515_can.h"

#define led_baude_rate 7
#define led_reception 6

//////////////////////prototype des fonction
void baude_rate_detection();

/////////////////////////init des port spi
#define MAX_DATA_SIZE 8
const int SPI_CS_PIN = 10;
const int CAN_INT_PIN = 2;
mcp2515_can CAN(SPI_CS_PIN); // Set CS pin


////////////////////// les variable
char *vitesse_can[13]={"5KBPS","10KBPS","20KBPS","40KBPS","50KBPS","80KBPS","100KBPS","125KBPS","200KBPS","250KBPS","500KBPS","800KBPS","1000KBPS"};
int incr=0;
unsigned long  temps1=millis();




//////////////////////////////////////////////////////////
void setup() {
  
    pinMode(led_baude_rate,OUTPUT);
    pinMode(led_reception,OUTPUT);
    digitalWrite(led_baude_rate,LOW);
    digitalWrite(led_reception,LOW);

    SERIAL_PORT_MONITOR.begin(115200);
    while (!SERIAL_PORT_MONITOR) {}

 
                 CAN.setMode(CAN_NORMAL_MODE);
    

    while (CAN_OK != CAN.begin(CAN_500KBPS,MCP_8MHz)) {             // init can bus : baudrate = 500k
        SERIAL_PORT_MONITOR.println(F("CAN init fail, retry..."));
        delay(100);
    }
    
    SERIAL_PORT_MONITOR.println(F("CAN init ok!"));
    //CAN.begin(CAN_250KBPS,MCP_8MHz);

    
    //////////detection de vitesse de can
    baude_rate_detection();
    SERIAL_PORT_MONITOR.print("vitesse detecter est = ");
    SERIAL_PORT_MONITOR.println(vitesse_can[incr-1]);
}


///////////les variable de bus can
uint32_t id;
uint8_t  type; // bit0: ext, bit1: rtr
uint8_t  len;
byte cdata[MAX_DATA_SIZE] = {0};

void loop() {
 
    // check if data coming
    if (CAN_MSGAVAIL != CAN.checkReceive()) {
       digitalWrite(led_reception,LOW);
        return;
    }
    char prbuf[32 + MAX_DATA_SIZE * 3];
    int i, n;
     digitalWrite(led_reception,HIGH);
   // unsigned long t = millis();
    // read data, len: data length, buf: data buf
    CAN.readMsgBuf(&len, cdata);

    id = CAN.getCanId();
    type = (CAN.isExtendedFrame() << 0) |
           (CAN.isRemoteRequest() << 1);
    

    //n = sprintf(prbuf, "%04lu.%03d ", t / 1000, int(t % 1000));
    /* Displayed type:
     *
     * 0x00: standard data frame
     * 0x02: extended data frame
     * 0x30: standard remote frame
     * 0x32: extended remote frame
     */
    static const byte type2[] = {0x00, 0x02, 0x30, 0x32};
    n= sprintf(prbuf, "RX: ID=[%lX] DLC=[%d] DATA= ", (unsigned long)id,len);
    // n += sprintf(prbuf, "RX: [%08lX](%02X) ", id, type);

    for (i = 0; i < len; i++) {
        n += sprintf(prbuf + n, "%02X ", cdata[i]);
    }
    SERIAL_PORT_MONITOR.println(prbuf);
}




//////////////////////////////////////////////////////////////
void baude_rate_detection(){

  while (CAN_MSGAVAIL != CAN.checkReceive()) {
    
       
    
    if(millis()-temps1>=200){
          switch(incr){
              case 0:
                     CAN.begin(CAN_5KBPS,MCP_8MHz);
                     break;
              case 1:
                     CAN.begin(CAN_10KBPS,MCP_8MHz);
                     break;
              case 2:
                     CAN.begin(CAN_20KBPS,MCP_8MHz);
                     break;
              case 3:
                     CAN.begin(CAN_40KBPS,MCP_8MHz);
                     break;
              case 4:
                     CAN.begin(CAN_50KBPS,MCP_8MHz);
                     break;
              case 5:
                     CAN.begin(CAN_80KBPS,MCP_8MHz);
                     break;
              case 6:
                     CAN.begin(CAN_100KBPS,MCP_8MHz);
                     break;
              case 7:
                     CAN.begin(CAN_125KBPS,MCP_8MHz);
                     break;
              case 8:
                     CAN.begin(CAN_200KBPS,MCP_8MHz);
                     break;
              case 9:
                     CAN.begin(CAN_250KBPS,MCP_8MHz);
                     break;
              case 10:
                     CAN.begin(CAN_500KBPS,MCP_8MHz);
                     break; 
              case 11:
                     CAN.begin(CAN_800KBPS,MCP_8MHz); 
                     break; 
              case 12:
                     CAN.begin(CAN_1000KBPS,MCP_8MHz);
                     break;
                                                                                           
          }
       incr++;
       temps1=millis();
       
       if(incr==13){
        incr=0;
       }
       
       SERIAL_PORT_MONITOR.println(incr);  
    }
      
    }
    digitalWrite(led_baude_rate,HIGH);
}
 
// END FILE
