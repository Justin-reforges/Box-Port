#include <SoftwareSerial.h>
 
int bluetoothTx = 0;  // TX-O pin of bluetooth mate, Arduino D2
int bluetoothRx = 1;  // RX-I pin of bluetooth mate, Arduino D3
 
int armLanding = 11;
int armCharging = 12;
int armMotor = 13;
int movePlatform;

int direction;    // Variable to set Rotation (CW-CCW) of the motor
int steps = 0; // Assumes the belt clip is in the Middle
int customDelay,customDelayMapped; 

#define step_pin 5  // Pin 3 connected to Steps pin on EasyDriver
#define dir_pin 4   // Pin 2 connected to Direction pin
#define step_pin2 6  // Pin 3 connected to Steps pin on EasyDriver
#define dir_pin2 7   // Pin 2 connected to Direction pin
 
int dataFromBt;
 
boolean lightBlink = false;
 
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);
 
void setup()
{
  Serial.begin(9600);  // Begin the serial monitor at 9600bps
 
  bluetooth.begin(115200);  // The Bluetooth Mate defaults to 115200bps
  bluetooth.print("$");  // Print three times individually
  bluetooth.print("$");
  bluetooth.print("$");  // Enter command mode
  delay(100);  // Short delay, wait for the Mate to send back CMD
  bluetooth.println("U,9600,N");  // Temporarily Change the baudrate to 9600, no parity
  // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
  bluetooth.begin(9600);  // Start bluetooth serial at 9600
  pinMode(armLanding, OUTPUT);
  pinMode(armCharging, OUTPUT);
  pinMode(armMotor, OUTPUT);
  digitalWrite(armMotor, HIGH);
  digitalWrite(armLanding, HIGH);
  digitalWrite(armCharging, HIGH);

}
 
void loop()
{
 
//  if (bluetooth.available()) // If the bluetooth sent any characters
 // {
    // Send any characters the bluetooth prints to the serial monitor
 
    dataFromBt = bluetooth.read();
 
    //Serial.println(dataFromBt), 0 = ArmMotor
    if (dataFromBt == '0') {
      Serial.println("Armed");
      digitalWrite(armMotor, LOW);
      bluetooth.print("Armed");
    }
    // 1 = DisarmMotor
    if (dataFromBt == '1') {
      Serial.println("Disarmed");
      digitalWrite(armMotor, HIGH);
      bluetooth.print("Disarmed");
    }
    // 2 = armLanding
    if (dataFromBt == '2') {
      Serial.println("Landing ON");
      digitalWrite(armLanding, LOW);
      bluetooth.print("2");
    }

    // 3 = disarmLanding
    if (dataFromBt == '3') {
      Serial.println("Landing OFF");
      digitalWrite(armLanding, HIGH);
      bluetooth.print("3");
    }
    // 4 = armCharging
    if (dataFromBt == '4') {
      Serial.println("Charging ON");
      digitalWrite(armCharging, LOW);
      bluetooth.print("4");
    }

     // 5 = DisCharging
    if (dataFromBt == '5') {
      Serial.println("Charging OFF");
      digitalWrite(armCharging, HIGH);
      bluetooth.print("5");
    }
    
    // 6 = UPPlatform
    if (dataFromBt == '6') {
      Serial.println("Platform UP");
      movePlatform == HIGH;
      bluetooth.print("6");

      while (steps < 9300) {
        customDelayMapped = speedUp(); 
        digitalWrite(dir_pin, HIGH);
        digitalWrite(dir_pin2, HIGH);
        digitalWrite(step_pin, HIGH);
        digitalWrite(step_pin2, HIGH);
        delayMicroseconds(800);
        digitalWrite(step_pin, LOW);
        digitalWrite(step_pin2, LOW);
        delayMicroseconds(800);
        steps++;
      }
    }

    // 7 = DownPlatform
    if (dataFromBt == '7') {
      Serial.println("Platform DOWN");
      movePlatform == LOW;
      bluetooth.print("7");
     while (steps > 0) {
      customDelayMapped = speedUp(); 
      digitalWrite(dir_pin, LOW);  // (HIGH = anti-clockwise / LOW = clockwise)
      digitalWrite(dir_pin2, LOW);
      digitalWrite(step_pin, HIGH);
      digitalWrite(step_pin2, HIGH);
      delayMicroseconds(800);
      digitalWrite(step_pin, LOW);
      digitalWrite(step_pin2, LOW);
      delayMicroseconds(800);
      steps--;
          }
    }

    
}
 

// Function for reading the Potentiometer
int speedUp() {
  
  int customDelay = map(steps, 0, 999, 100 ,0); // Reads the potentiometer
  int customDelay1 = 0;
  int customDelay2 = map(steps, 8000, 9299, 0 ,100); // Reads the potentiometer

  int newCustom;
  
  if (steps <1000) {
      newCustom = map(customDelay, 0, 100, 500,1000); // Convrests the read values of the potentiometer from 0 to 1023 into desireded delay values (300 to 4000)
  }
    if (steps > 8000) {
      newCustom = map(customDelay2, 0, 100, 500,1000);
    }
    if (steps >= 1000 && steps <= 8000) {
  newCustom = map(customDelay1, 0, 100, 500,1000); // Convrests the read values of the potentiometer from 0 to 1023 into desireded delay values (300 to 4000)
  }
  return newCustom;  
}


