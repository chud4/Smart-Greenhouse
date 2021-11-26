/*
 * SMRTTECH 4SC3 - C01 - Course Project
 * Jack Holmes, Daniew Chu, Mathew Wu
 * Nov 12, 2021
*/

// Set Board to CubeCell-Board (HTCC-AB01).
// *NOTE: Serial communication between Arduino Uno and CubeCell cannot occur while the two are connected via Tx/Rx.

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
char txpacket[BUFFER_SIZE];
char serialIN[BUFFER_SIZE];
static RadioEvents_t RadioEvents;
double txNumber;
int16_t rssi,rxSize;
void  DoubleToString( char *str, double double_num,unsigned int len);

void setup() {
  Serial.begin(115200);

  txNumber=0;
  rssi=0;

  Radio.Init( &RadioEvents );
  Radio.SetChannel( RF_FREQUENCY );
  Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH, LORA_SPREADING_FACTOR, LORA_CODINGRATE, LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON, true, 0, 0, LORA_IQ_INVERSION_ON, 3000); 
}

void loop()
{
  ReadFromArduino();
  delay(1000);
  txNumber += 0.01;
  sprintf(txpacket,"%s",serialIN);                        // Start a package.
  
  turnOnRGB(COLOR_SEND,0);                                // Change rgb color.

  Radio.Send( (uint8_t *)txpacket, strlen(txpacket) );    // Send the package out.

  Serial.println(txpacket);                               // Serial print in order to ensure packet is correct.
}

/**
  * @brief  Double To String
  * @param  str: Array or pointer for storing strings
  * @param  double_num: Number to be converted
  * @param  len: Fractional length to keep
  * @retval None
  */
void DoubleToString( char *str, double double_num,unsigned int len) {      // Used in the event that any additional information of type 'double' is needed to be sent.
  double fractpart, intpart;
  fractpart = modf(double_num, &intpart);
  fractpart = fractpart * (pow(10,len));
  sprintf(str + strlen(str),"%d", (int)(intpart));                          // Integer part.
  sprintf(str + strlen(str), ".%d", (int)(fractpart));                      // Decimal part.
}

void ReadFromArduino(){                                                     // Use this function to read the string from the arduino.
  Serial.readBytes(serialIN,BUFFER_SIZE);                                   // Read the serial data and store in var.
}
