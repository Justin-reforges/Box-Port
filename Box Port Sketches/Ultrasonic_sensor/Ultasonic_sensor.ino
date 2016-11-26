/**
 *  connections:
 *  arduino pin | ultrasound
 *  -------------------
 *      5V      |   vcc   
 *      7       |   trig     
 *      8       |   echo     
 *      gnd     |   gnd
 */

// Pins
const int TRIG_PIN = 7;
const int ECHO_PIN = 8;

const unsigned int MAX_DIST = 23200; // max range of 400cm

void setup() {
  // The Trigger pin will tell the sensor to range find
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);

  Serial.begin(9600);
}

void loop() {

  unsigned long t1;
  unsigned long t2;
  unsigned long pulse_width;
  float cm;
  float inches;

  // hold the trigger pin high for at least 10 us
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // wait for pulse reflection to reach echo pin
  while ( digitalRead(ECHO_PIN) == 0 );

  // measure how long the echo pin was held high (pulse width)
  t1 = micros();
  while ( digitalRead(ECHO_PIN) == 1);
  t2 = micros();
  pulse_width = t2 - t1;

  // distance = (echo time) * (speed of sound) / 2
  // cm       =   t2-t1     *     343m/s       / 2 
  cm = pulse_width * 0.01715;

  // print results
  if ( pulse_width > MAX_DIST ) {
    Serial.println("Out of range");
  } else {
    Serial.print(cm);
    Serial.print(" cm \t");
    Serial.print(inches);
    Serial.println(" in");
  }
  
  // Wait at least 60ms before next measurement
  delay(60);
}
