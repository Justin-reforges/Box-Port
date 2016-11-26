/* last updated 19 October, 2016
 *  Written by Isaac D'Souza
 *  updated by Justin Cechmanek
 */
#include <SoftwareSerial.h> 

#define X_pin A4    // Pin A4 connected to joystick x axis
#define y_pin A5    // Pin A5 connected to joystick y axis
#define Joy_switch 9  // Pin 4 connected to joystick switch
#define echoPin 13 // Echo Pin
#define trigPin 12 // Trigger Pin


int bluetoothTx = 11; // TX pin of bluetooth mate
int bluetoothRx = 10; // RX pin of bluetooth mate
int bluetoothCmd;
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

//M1 is Apperture Motor
//M2 is Lift Motor
int M1dirpin = 4;
int M1steppin = 5;
int M2dirpin = 7;
int M2steppin = 6;
int relayPin = 2;
int cmd = 0;
int mode = 0; 
/* 'mode' is state of port 
 *  0 = stopall(). kill button
 *  10 = joystick manual port control
 *  20 = * not used currently
 *  30 = port closed, can deploy() or reteive() 
 *  40 = drone in, ready to charge
 *  50 = port open, landing pad raised
 */
int hallSwitch = A0;
int hallValue = 0; // hall sensor reading
int irisOpenValue = 0;// 510;
int irisCloseValue = 900;// 552;
int minimumRange = 0; // Minimum range needed
int maximumRange = 50;
int storagePosition = 19;
int centeringPosition = 26;
int launchPosition = 30;
long previousdistance, duration, distance; // Duration used to calculate distance
boolean irisOpen, irisClosed, landingPadUp, landingPadDown, droneCentered;


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
  pinMode(M1dirpin,OUTPUT);
  pinMode(M1steppin,OUTPUT);
  pinMode(M2dirpin,OUTPUT);
  pinMode(M2steppin,OUTPUT);
  pinMode(Joy_switch, INPUT_PULLUP);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(relayPin,OUTPUT);
  for (int i=0;i<50;i++){
    getRange();
  }
}

void getRange(){
  boolean inRange = false;
  while(!inRange){
    digitalWrite(trigPin, LOW); 
    delayMicroseconds(2); 

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10); 
 
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
 
    //Calculate the distance (in cm) based on the speed of sound.
    //previousdistance=distance;
    previousdistance = duration/58.2; 
    delay(10); 
    digitalWrite(trigPin, LOW); 
    delayMicroseconds(2); 

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10); 
 
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
 
    //Calculate the distance (in cm) based on the speed of sound.
    //previousdistance=distance;
    distance = duration/58.2;
    if (distance <= maximumRange && distance >= minimumRange){
      if (distance < (previousdistance+1) && distance > (previousdistance-1)) {
        inRange = true; 
      }
    }
    delay(10);
  } 
  Serial.print(distance);
  Serial.print(",");
  Serial.println(previousdistance);
}

void updateRange(){
  int range;
  getRange();
  range = distance;
  getRange();
  range += distance;
  range /= 2;
  distance = range;
}

void openIris(){
  //Serial.println("opening");
  digitalWrite(M1dirpin,LOW);
  analogWrite(M1steppin,50);
}

void closeIris(){
  //Serial.println("closing");
  digitalWrite(M1dirpin,HIGH);
  analogWrite(M1steppin,50);
}

void liftPad(){
  digitalWrite(M2dirpin,LOW);
  analogWrite(M2steppin,50);
}

void stopAll(){
  analogWrite(M2steppin,0);
  analogWrite(M1steppin,0);
}

void lowerPad(){
  digitalWrite(M2dirpin,HIGH);
  analogWrite(M2steppin,50);
}

void getCmd() {
  if (Serial.available() > 0) {
    // read the incoming byte:
    mode = Serial.parseInt();
    cmd = Serial.parseInt();
  }
}

void deploy() {
  while (!irisOpen) {
    hallValue = analogRead(hallSwitch);
    if (hallValue > irisOpenValue) {
      openIris();
    }
    else {
      irisOpen = true;
      stopAll();
    }
  }
    
  while (!landingPadUp) {
    getRange();
    if (distance > launchPosition) {
      //Serial.println(distance);
      landingPadUp = true;
      stopAll();
    }
    else {
      liftPad();
    }  
  }
}

void retrieve() {
  while (!landingPadDown) {
    getRange();
    if (distance < centeringPosition) {
      //Serial.println(distance);
      landingPadDown = true;
      stopAll();
    }
    else {
      lowerPad();
    }
  }
  closeIris();
  delay(1000);
  stopAll();

  irisOpen = false;
  while (!irisOpen) {
    hallValue = analogRead(hallSwitch);
    if (hallValue > irisOpenValue) {
      openIris();
    }
    else {
      irisOpen = true;
      stopAll();
    }
  }
    
  landingPadDown = false;
  while (!landingPadDown) {
    getRange();
    if (distance < storagePosition) {
      //Serial.println(distance);
      landingPadDown = true;
      stopAll();
    }
    else {
      lowerPad();
    }
  }
  while (!irisClosed) {
    hallValue = analogRead(hallSwitch);
    if (hallValue > irisCloseValue) {
      irisClosed = true;
    }
    else {
      closeIris();
    }
  }
}
  
void center() {
  
}

void relayOn() {
  digitalWrite(relayPin,HIGH);  
}

void relayOff() {
  digitalWrite(relayPin,LOW);
}

/*
 * Main loop
 */
void loop() {
  // send data only when you receive data:
  getCmd();

  // Emergency stop
  if (mode == 0) {
    stopAll(); // stops iris motor and lifting pad motor
    relayOff(); // stops wireless charger
  }

  //  Manual Control via joystick
  if (mode == 10) {
    if (analogRead(X_pin) > 900) {  //  If joystick is moved Left
      hallValue = analogRead(hallSwitch);
      if (hallValue < irisOpenValue){
        stopAll(); 
      }
      else {
        openIris();
      }
    }
    if (analogRead(X_pin) < 300) {  //  If joystick is moved Right
      hallValue = analogRead(hallSwitch);
      if (hallValue > 540){ // why 540 here and 510 or 552 elsewhere?
        stopAll();
      }
      else {
        closeIris();
      }
    }
    
    if (analogRead(y_pin) > 900) {  //  If joystick is moved up
      liftPad();
    }
    if (analogRead(y_pin) < 300) {  //  If joystick is moved down
      lowerPad();
    }
    if (cmd == 0) {
      stopAll();
    }
  } // end mode 10

  // Port closed
  if (mode == 30) {
    if (cmd == 0) { // deploy drone, go to mode 50-drone on open port
      relayOff();
      irisOpen = false;
      landingPadUp = false;
      deploy();
      stopAll();
      mode = 50;
    }
    if(cmd == 1) { // retrieve drone, go to mode 50-drone on open port
      irisClosed = false;
      landingPadDown = false;
      retrieve();
      stopAll();
      relayOn();
      mode = 50;
    }
  } // end mode 30

  // Drone in port ready to charge
  if (mode == 40) {
    if (cmd == 0){
      Serial.println("relay ON");
      relayOn(); // charging drone
    }
    if (cmd == 1) {
      Serial.println("relay Off");
      relayOff(); // stop charging
    }
  } // end mode 40

  if (bluetooth.available()) {
    bluetoothCmd = bluetooth.read();
    Serial.println(bluetoothCmd);
    switch(bluetoothCmd) { // cases correspond to buttons in app
      case 50: // deploy
        mode = 30;
        cmd = 0;
        break;
      case 51: // retrieve
        mode = 30;
        cmd = 1;
        break;
      case 52: // charging on
        mode = 40;
        cmd = 0;
        break;
      case 53: // charging off
        mode = 40;
        cmd = 1;
        break;
      case 54: // lift pad
        mode = 10;
        cmd = 2;
        break;
      case 55: // lower pad
        mode = 10;
        cmd = 3;
        break;
      case 56: // take off
        mode = 50;
        cmd = 1;
        break;
      case 57: // land
        mode = 50;
        cmd = 2;
        break;
      case 58: // open iris
        mode = 10;
        cmd = 0;
        break;
      case 59: // close iris
        mode = 10;
        cmd = 1;
        break;
      case 60: // stop all
        mode = 0;
        cmd = 0;
        break;
      case 61: // one button launch
        // stop charging, open iris, raise platform, launch, lower platform, close iris
        break;
      default:
        Serial.println("unknown command received from app");
        break;
    } // end switch(bluetoothCmd)
  } // end if(bluetooth.available())
  
} // end main loop

