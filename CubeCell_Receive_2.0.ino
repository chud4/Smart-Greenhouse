/*
 * SMRTTECH 4SC3 - C01 - Course Project
 * Jack Holmes, Daniew Chu, Mathew Wu
 * Nov 12, 2021
*/

//1baa1ac0-c947-4201-aed9-a766e0c72ac7
//20YMF3MBWXPZX9GCHT7N

// Set Board to CubeCell-Board (HTCC-AB01).

// Function Definitions
#include "LoRaWan_APP.h"
#include "Arduino.h"

#ifndef LoraWan_RGB
#define LoraWan_RGB 0
#endif

#define RF_FREQUENCY                                915000000 // Hz
#define TX_OUTPUT_POWER                             14        // dBm
#define LORA_BANDWIDTH                              0         // [0: 125 kHz,
                                                              //  1: 250 kHz,
                                                              //  2: 500 kHz,
                                                              //  3: Reserved]
#define LORA_SPREADING_FACTOR                       7         // [SF7..SF12]
#define LORA_CODINGRATE                             1         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         0         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
#define LORA_IQ_INVERSION_ON                        false
#define RX_TIMEOUT_VALUE                            1000
#define BUFFER_SIZE                                 30 // Define the payload size here

// Global Variable Declaration
char rxpacket[BUFFER_SIZE];
static RadioEvents_t RadioEvents;
int16_t txNumber;
int16_t rssi,rxSize;
String rxString;
char Data[5][BUFFER_SIZE];

void setup() {
  Serial.begin(115200);

  txNumber=0;
  rssi=0;

  RadioEvents.RxDone = OnRxDone;
  Radio.Init( &RadioEvents );
  Radio.SetChannel( RF_FREQUENCY );

  Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR, LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH, LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON, 0, true, 0, 0, LORA_IQ_INVERSION_ON, true);
  turnOnRGB(COLOR_SEND,0); //change rgb color
}

void loop()
{
  Radio.Rx( 0 );
  delay(500);
  Radio.IrqProcess( );

  char rxTemp[BUFFER_SIZE];
  rxString.toCharArray(rxTemp, rxString.length());
  //char DataTemp[BUFFER_SIZE];
  int layer = 0;
  //Serial.println(rxString);
  for(int i=0; i<=BUFFER_SIZE; i++){                // Delimit the data into sections based on ':'
    if(rxString.charAt(i)==':' || i==BUFFER_SIZE){
      layer++;          
    }else{
      Data[layer][i] = rxString.charAt(i);
      //Serial.print(Data[layer][i]);
    }
  }
  String Data1;
  String Data2;
  for(int j=0; j<BUFFER_SIZE; j++){
    Data1.concat(Data[1][j]);
  }
  for(int j=0; j<BUFFER_SIZE; j++){
    Data2.concat(Data[2][j]);
  }
  Serial.print(Data1);
  Serial.print(", ");
  Serial.print(Data2);
  Serial.print(", ");
  Serial.println();
}

void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
    rssi=rssi;
    rxSize=size;
    memcpy(rxpacket, payload, size );
    rxpacket[size]='\0';
    turnOnRGB(COLOR_RECEIVED,0);
    Radio.Sleep( );
    //Serial.println(rxpacket);
    rxString = String(rxpacket);
}
