/* last updated 22 November, 2016
 *  Written by Justin Cechmanek
 */
#include <SoftwareSerial.h> 
#define bluetoothTx 11 // TX pin of bluetooth mate
#define bluetoothRx 10 // RX pin of bluetooth mate
int bluetoothCmd, serialCmd, command;
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

#define X_pin A4    // Pin A4 connected to joystick x axis
#define Y_pin A5    // Pin A5 connected to joystick y axis
#define Joy_switch 9  // Pin 4 connected to joystick switch

#define irisOpenSwitch 0
#define irisClosedSwitch 1
#define platformUpSwitch 2
#define platformDownSwitch 3

//M1 is iris motor
//M2 is platform motor
#define M1dirPin 4
#define M1stepPin 5
#define M2dirPin 7
#define M2stepPin 6

int fullOpenSteps = 500;
int fullClosedSteps = 500;
int fullUpSteps = 500;
int fullDownSteps = 500;

boolean irisOpen, irisClosed, platformUp, platformDown, droneCentered;
boolean manualControl;

void setup(){
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
  pinMode(M1dirPin,OUTPUT);
  pinMode(M1stepPin,OUTPUT);
  pinMode(M2dirPin,OUTPUT);
  pinMode(M2stepPin,OUTPUT);
  pinMode(Joy_switch, INPUT_PULLUP);

  pinMode(irisOpenSwitch, INPUT);
  digitalWrite(irisOpenSwitch, HIGH); // turn on pull up resistor
  pinMode(irisClosedSwitch, INPUT);
  digitalWrite(irisClosedSwitch, HIGH); // turn on pull up resistor
  pinMode(platformUpSwitch, INPUT);
  digitalWrite(platformUpSwitch, HIGH); // turn on pull up resistor
  pinMode(platformDownSwitch, INPUT);
  digitalWrite(platformDownSwitch, HIGH); // turn on pull up resistor
}

void openIris(){
  //Serial.println("opening");
  irisClosed = false;
  digitalWrite(M1dirPin, LOW);
  int irisSteps = 0;
  while (!irisOpen){
    digitalWrite(M1stepPin, HIGH);
    delay(2);
    digitalWrite(M1stepPin, LOW);
    delay(2);
    irisSteps ++;
    //check if iris is open, or if motor moved enough to open, but switch didn't register
    irisOpen = !digitalRead(irisOpenSwitch);
    if (irisSteps >= fullOpenSteps) {
      irisOpen = true;
    }
  }
  //Serial.println("iris open");
}

void closeIris(){
  //Serial.println("closing");
  irisOpen = false; 
  digitalWrite(M1dirPin, HIGH);
  int irisSteps = 0;
  while (!irisClosed){
    digitalWrite(M1stepPin, HIGH);
    delay(2);
    digitalWrite(M1stepPin, LOW);
    delay(2);
    irisSteps ++;
    //check if iris is closed, or if motor moved enough to close, but switch didn't register
    irisClosed = !digitalRead(irisClosedSwitch);
    if (irisSteps >= fullClosedSteps) {
      irisClosed = true;
    }
  }
  //Serial.println("iris closed");
}

void liftPad(){
  //Serial.println("lifting");
  platformDown = false; 
  digitalWrite(M2dirPin, HIGH);
  int platformSteps = 0;
  while (!platformUp){
    digitalWrite(M2stepPin, HIGH);
    delay(2);
    digitalWrite(M2stepPin, LOW);
    delay(2);
    platformSteps ++;
    //check if platform is up, or if motor moved enough to lift, but switch didn't register
    platformUp = !digitalRead(platformUpSwitch);
    if (platformSteps >= fullUpSteps) {
      platformUp = true;
    }
  }
  //Serial.println("platform raised");
}

void lowerPad(){
  //Serial.println("lowering");
  platformUp = false; 
  digitalWrite(M2dirPin, LOW);
  int platformSteps = 0;
  while (!platformDown){
    digitalWrite(M2stepPin, HIGH);
    delayMicroseconds(2);
    digitalWrite(M2stepPin, LOW);
    delayMicroseconds(2);
    platformSteps ++;
    //check if platform is down, or if motor moved enough to lower, but switch didn't register
    platformDown = !digitalRead(platformDownSwitch);
    if (platformSteps >= fullDownSteps) {
      platformDown = true;
    }
  }
  //Serial.println("platform lowered");
}

void stopAll(){
  analogWrite(M2stepPin,0);
  analogWrite(M1stepPin,0);
}

int getSerialCmd() {
  if (Serial.available()) {
    // read the incoming byte:
    serialCmd = Serial.parseInt();
    manualControl = false;
    return serialCmd;
  }
  return 0;
}

int getBluetoothCmd() {
  if (bluetooth.available()) {
    bluetoothCmd = bluetooth.read();
    manualControl = false;
    return bluetoothCmd;
  }
  return 0;
}

void center() {
  
}

/* Main loop */
void loop() {
  
  command = getSerialCmd();
  if (!command){ // if no serial command was found
    command = getBluetoothCmd();
  }

  switch (command) {

    case 0: // loop through procedure
      manualControl = true;
      break;
    case 1: // emergency stop
      stopAll();
      break;
    case 2: 
      closeIris();
      break;
    case 3:
      openIris();
      break;
    case 4:
      liftPad();
      break;
    case 5:
      lowerPad();
      break;
    default: // loop through procedure
      break;
  }
    
  //  Manual Control via joystick
  if (manualControl) {
    //while ( analogRead(X_pin)<300 && !digitalRead(irisOpenSwitch) && !digitalRead(irisClosedSwitch) ) {// If joystick moved Right
    while ( analogRead(X_pin)<300){
      digitalWrite(M1dirPin, HIGH);
      digitalWrite(M1stepPin, HIGH);
      delay(2);
      digitalWrite(M1stepPin, LOW);
      delay(2);
    }
    //while ( analogRead(X_pin)>900 && !digitalRead(irisOpenSwitch) && !digitalRead(irisClosedSwitch) ) {// If joystick moved Left
    while ( analogRead(X_pin)>900){
      digitalWrite(M1dirPin, LOW);
      digitalWrite(M1stepPin, HIGH);
      delay(1);
      digitalWrite(M1stepPin, LOW);
      delay(1);
    }
    //while ( analogRead(Y_pin)<300 && !digitalRead(platformUpSwitch) && !digitalRead(platformDownSwitch) ) {// If joystick moved Forward
    while ( analogRead(Y_pin)<300){
      digitalWrite(M2dirPin, HIGH);
      digitalWrite(M2stepPin, HIGH);
      delay(1);
      digitalWrite(M2stepPin, LOW);
      delay(1);
    }
    //while ( analogRead(Y_pin)>900 && !digitalRead(platformUpSwitch) && !digitalRead(platformDownSwitch) ) {// If joystick moved Backward
    while ( analogRead(Y_pin)>900){
      digitalWrite(M2dirPin, LOW);
      digitalWrite(M2stepPin, HIGH);
      delay(1);
      digitalWrite(M2stepPin, LOW);
      delay(1);
    }
  } // end manual joystick control

  else {
    lowerPad();
    delay(10);
    closeIris();
    delay(5000);
    openIris();
    delay(10);
    liftPad();
    delay(10000);
  }
 
} // end main loop

