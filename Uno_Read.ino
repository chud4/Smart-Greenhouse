/*
 * SMRTTECH 4SC3 - C01 - Course Project
 * Jack Holmes, Daniew Chu, Mathew Wu
 * Nov 12, 2021
*/

// Set Board to Arduino Uno and connect via port.
// *NOTE: Serial communication between Arduino Uno and CubeCell cannot occur while the two are connected via Tx/Rx.

// Variable Initialization
#define BUFFER_SIZE 30 // Needs to be constant across each program.
char serialOUT[BUFFER_SIZE];
String temp1;
String temp2;
String packet;
int STR_LENGTH;
int FILLER;

// Pin Designation
int LM34Pin1 = A2;
int LM34Pin2 = A3;

void setup() {
  Serial.begin(115200); // 115200 baud
  
  // Pin Assignment
  pinMode(LM34Pin1, INPUT);
  pinMode(LM34Pin2, INPUT);
}

void loop() {
  double LM34Val1 = LM34Run(0);                         // First LM34 Read.
  temp1 = String(LM34Val1,2);                           // Convert to String.
  double LM34Val2 = LM34Run(1);                         // Second LM34 Read.
  temp2 = String(LM34Val2,2);                           // Convert to String.

  packet = ":" + temp1 + ":" + temp2 + ":";             // Collect all data strings into a single output string using ':' to separate data chunks.
  STR_LENGTH = (packet.length() + 1);                   // Determing the current length of output string.
  FILLER = (BUFFER_SIZE - STR_LENGTH);                  // Calculate the amount of filler needed to reach the desired BUFFER_SIZE.
  for(int i=0; i<FILLER; i++){                          // Use this loop to fill the remaining space with ' ' characters to be trimmed upon receipt.
    packet = packet + " ";
  }
  Serial.println(packet);                               // Print string to serial monitor. This is directly read on CubeCell. Any other 'Serial.print' lines will also be attached, so for official use have only this one.
  delay(1000);                                          // 1 second delay each loop.
}

double LM34Run(int Case){
  double LM34Read;

  if(Case == 0){                                        // Sensor 1 or 2?
    LM34Read = analogRead(LM34Pin1);                    // Collect a reading.
  }else{
    LM34Read = analogRead(LM34Pin2);                    // Collect a reading.
  }
  //Serial.println(IntLM34Read);
                                                        // This entire section is dependent on the sensor being used. In this case it is two LM34 sensors.
  double MillivoltRead = (LM34Read/1024.0)*5000.0;      // Convert LM34 value to millivoltage range.
  double Fahrenheit = MillivoltRead/10;                 // Fahrenheit Calculation.
  double Celsius = (Fahrenheit-32) * (5.0/9.0);         // Celsius Calculation.

  // Select one of the following to return.
  return Celsius;
  //return Fahrenheit;
}
