/* last updated 19 October, 2016
 *  Written by Isaac D'Souza
 *  updated by Justin Cechmanek
 */
#include <SoftwareSerial.h> 

int bluetoothTx = 11; // TX pin of bluetooth mate
int bluetoothRx = 10; // RX pin of bluetooth mate
int bluetoothCmd;
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

void setup(){
  // plug bluetooth transciever into pins 8,9,10,11, where 8 and 9 act as ground and power
  pinMode(9,OUTPUT);
  digitalWrite(9, LOW);
  pinMode(8,OUTPUT);
  digitalWrite(8, HIGH);
  
  // setup bluetooth connection
  bluetooth.begin(115200);  // The Bluetooth Mate defaults to 115200bps
  bluetooth.print("$");  // Print three times individually
  bluetooth.print("$");
  bluetooth.print("$");  // Enter command mode
  delay(100);  // Short delay, wait for the Mate to send back CMD
  bluetooth.println("U,9600,N");  // Temporarily Change the baudrate to 9600, no parity
  // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
  bluetooth.begin(9600);  // Start bluetooth serial at 9600
 
  // setup pin IO
  Serial.begin(9600);
}
void loop(){
  
  if (bluetooth.available()) {
    bluetoothCmd = bluetooth.read();
    Serial.println(bluetoothCmd);
  }
} // end main loop

