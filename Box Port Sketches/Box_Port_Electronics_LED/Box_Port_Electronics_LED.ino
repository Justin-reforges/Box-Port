//NeoPixel library for LED strip control
#include <Adafruit_NeoPixel.h>
#define LED_STRIP_PIN      6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);
#define LED_STRIP_POWER    8 // pwm pin connected to D8 output block on RAMPS board

#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#define Y_MIN_PIN          14
#define Y_MAX_PIN          15

#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62
#define Z_MIN_PIN          18
#define Z_MAX_PIN          19

#define E_STEP_PIN         26
#define E_DIR_PIN          28
#define E_ENABLE_PIN       24

#define LED_PIN            13

//endstops

#define END_LID_OP           3 //1
#define END_LID_CL           2 //2
#define END_LOAD_TOP        14 //3
#define END_LOAD_BOT        15 //4
#define END_CENTER          18 //5

//Integer Values
int value = -1;
int safe = -2;
float input = -3;
int delayTime = 2;

void setup() {

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  pinMode(LED_STRIP_POWER  , OUTPUT);
  digitalWrite(LED_STRIP_POWER  , HIGH); 
  
  pinMode(LED_PIN  , OUTPUT);
  
  pinMode(X_STEP_PIN  , OUTPUT);
  pinMode(X_DIR_PIN    , OUTPUT);
  pinMode(X_ENABLE_PIN    , OUTPUT);
  
  pinMode(Y_STEP_PIN  , OUTPUT);
  pinMode(Y_DIR_PIN    , OUTPUT);
  pinMode(Y_ENABLE_PIN    , OUTPUT);
  
  pinMode(Z_STEP_PIN  , OUTPUT);
  pinMode(Z_DIR_PIN    , OUTPUT);
  pinMode(Z_ENABLE_PIN    , OUTPUT);
  
  pinMode(E_STEP_PIN  , OUTPUT);
  pinMode(E_DIR_PIN    , OUTPUT);
  pinMode(E_ENABLE_PIN    , OUTPUT);
  
    
   //Enabling stepper motors
   digitalWrite(X_ENABLE_PIN    , LOW);
   digitalWrite(Y_ENABLE_PIN    , LOW);
   digitalWrite(Z_ENABLE_PIN    , LOW);
   digitalWrite(E_ENABLE_PIN    , LOW);
   
   //Endstops
   pinMode(END_LID_OP, INPUT);
   pinMode(END_LID_CL, INPUT);
   pinMode(END_LOAD_TOP, INPUT);
   pinMode(END_LOAD_BOT, INPUT);
   pinMode(END_CENTER, INPUT);

   //Bluetooth
   Serial.begin(9600);
   if(Serial.available()>0)
   {
    Serial.println("Bluetooth connection initiated.");   
   }
}


//Needed Functions

void openLid(){
   while(value == 1)
   {
   value = digitalRead(END_LID_OP);
   digitalWrite(X_DIR_PIN, HIGH);
   digitalWrite(Y_DIR_PIN, LOW);
   digitalWrite(X_STEP_PIN, HIGH);
   digitalWrite(Y_STEP_PIN, HIGH);
   
   delay(delayTime);
       
   digitalWrite(Y_STEP_PIN, LOW);
   digitalWrite(X_STEP_PIN,LOW);
   }
}

void closeLid(){
   while(value == 1 )
   {
   value = digitalRead(END_LID_CL);
   digitalWrite(X_DIR_PIN, LOW);
   digitalWrite(Y_DIR_PIN, HIGH);
   digitalWrite(X_STEP_PIN, HIGH);
   digitalWrite(Y_STEP_PIN, HIGH);
   
   delay(delayTime);
   
   digitalWrite(Y_STEP_PIN, LOW);
   digitalWrite(X_STEP_PIN,LOW);
   }
}

void platformUp(){
   while(value == 1)
   {
    value = digitalRead(END_LOAD_TOP);
    digitalWrite(E_DIR_PIN, HIGH);
    digitalWrite(E_STEP_PIN, HIGH);
    digitalWrite(Z_DIR_PIN, HIGH);
    digitalWrite(Z_STEP_PIN, HIGH);
    
    delay(delayTime);
    
    digitalWrite(Z_STEP_PIN, LOW);
    digitalWrite(E_STEP_PIN, LOW);
   }
 }

void platformDown(){
   while(value == 1 )
   {
    value = digitalRead(END_LOAD_BOT);

    digitalWrite(E_DIR_PIN, LOW);
    digitalWrite(Z_DIR_PIN, LOW);
    digitalWrite(Z_STEP_PIN, HIGH);
    digitalWrite(E_STEP_PIN, HIGH);
    
    delay(delayTime);
        
    digitalWrite(Z_STEP_PIN, LOW);
    digitalWrite(E_STEP_PIN, LOW);
   }
}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


/////////


void loop (){
    digitalWrite(LED_PIN, HIGH);
    
    if(Serial.available()>0)
    {
        input = Serial.read();
         //Full Open
        if(input == '1')
        {
            value = 1;
            openLid();
          
            delay(1000);
         
            value = 1;
            platformUp();
        }

        //Full Close
        else if(input == '2')
        {
            value = 1;
            platformDown();
        
            delay(1000);
        
            value = 1;
            closeLid();
        }

        //Lid Open
        else if(input == '3')
        {
            value = 1;
            openLid();
            delay(500);
        }

        //Lid Close
        else if(input == '4')
        {
            value = 1;
            closeLid();
            delay(500);
        }

        //Platform Rises
        else if(input == '5')
        {
            value = 1;
            platformUp();
            delay(500);
        }

        //Platform Lowers
        else if(input == '6')
        {
            value = 1;
            platformDown();
            delay(500);
        }

        else
        {
            digitalWrite(X_STEP_PIN    , LOW);
            digitalWrite(Y_STEP_PIN    , LOW);
            digitalWrite(Z_STEP_PIN    , LOW);

            for(int a = 0 ; a<10 ; a++);
            {
                digitalWrite(LED_PIN   , HIGH);
                digitalWrite(LED_PIN  , LOW);
            }

            Serial.println("Error");
        }
      
    }

}
 
