


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

void setup() {
  
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
   
   delayMicroseconds(400);
   
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
   
   delayMicroseconds(400);
   
   digitalWrite(Y_STEP_PIN, LOW);
   digitalWrite(X_STEP_PIN,LOW);
   }
}

void platformUp(){
   while(value == 1)
   {
    value = digitalRead(END_LOAD_TOP);
    
    digitalWrite(Z_DIR_PIN, HIGH);
    digitalWrite(Z_STEP_PIN, HIGH);
    
    delayMicroseconds(400);
    
    digitalWrite(Z_STEP_PIN,LOW);
   }
 }

void platformDown(){
   while(value == 1 )
   {
    value = digitalRead(END_LOAD_BOT);
    
    digitalWrite(Z_DIR_PIN, LOW);
    digitalWrite(Z_STEP_PIN, HIGH);
    
    delayMicroseconds(400);
    
    digitalWrite(Z_STEP_PIN,LOW);
   }
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
 
