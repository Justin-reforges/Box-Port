#include <SoftwareSerial.h> 

#define X_pin A4    // Pin A4 connected to joystick x axis
#define y_pin A5    // Pin A5 connected to joystick y axis
#define Joy_switch 9  // Pin 4 connected to joystick switch
#define echoPin 13 // Echo Pin
#define trigPin 12 // Trigger Pin

int bluetoothTx = 10;
int bluetoothRx = 11;
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
 *  50 = port open, landing pad raised?
 */

void setup(){

 bluetooth.begin(115200);
 bluetooth.print("S");
 bluetooth.print("S");
 bluetooth.print("S");
 delay(100);
 bluetooth.println("U,9600,N");
 bluetooth.begin(9600);


  // setup pin IO
  Serial.begin(9600);
  pinMode(M1dirpin,OUTPUT);
  pinMode(M1steppin,OUTPUT);
  pinMode(M2dirpin,OUTPUT);
  pinMode(M2steppin,OUTPUT);

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



/*
 * Main loop. 
 */
void loop() {


  if (bluetooth.available()) {
    bluetoothCmd = bluetooth.read();
    Serial.println(bluetoothCmd);
    Serial.println(" ");
    if (bluetoothCmd == 48) {
      lowerPad();
      delay(5000);
      stopAll();
    }
    else if (bluetoothCmd == 49) {
      liftPad();
      delay(5000);
      stopAll();
    }
    
    /*
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
    */
  } // end if(bluetooth.available())

} // end main loop

