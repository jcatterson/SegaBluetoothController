byte pin7 = 7;
byte pin1 = 9;
byte pin2 = 1;
byte pin3 = 4;
byte pin4 = 2;
byte pin6 = 3;
byte pin9 = 6;

int incomingByte = 0;
int counter = 0;
word pinsPressed;
word lastBtnsPressed;

typedef struct
{
    byte pin;
    char key;
    word idx;
} output;


output outputMap[] = {
    { pin1, 'U', 0x1},
    { pin2, 'D', 0x2},
    { pin3,  'L', 0x4},
    { pin4, 'R', 0x8},
    { pin6, 'A', 0x10},
    { pin6, 'B', 0x20},
    { pin9, 'S', 0x40},
    { pin9, 'C', 0x80}
};

const byte BUTTONS = 8;
byte lastHighStatus = 0;
byte lastLowStatus = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pin7, INPUT);
  pinMode(pin9, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);
  pinMode(pin6, OUTPUT);
  pinMode(pin9, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(pin7), writeControllerStatus, CHANGE);
  Serial.println("Done with setup");
}

void writeControllerStatus(){
  Serial.println("Pin interrupt");
  int selectPin = digitalRead(pin7);
  bool alreadyCleared = false;


  if(selectPin == HIGH){
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, HIGH);    
    digitalWrite(pin3, LOW);// Indicates controller connected
    digitalWrite(pin4, LOW);// Indicates controller connected
    digitalWrite(pin6, LOW);//(bool)(outputMap[4].idx & pinsPressed));// A
    digitalWrite(pin9, HIGH);// Start BTN
  }
  else{    
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, HIGH);
    digitalWrite(pin3, HIGH);
    digitalWrite(pin4, HIGH);//(bool)(outputMap[3].idx & pinsPressed));// R
    digitalWrite(pin6, HIGH);//(bool)(outputMap[5].idx & pinsPressed));// B
    digitalWrite(pin9, HIGH);//(bool)(outputMap[7].idx & pinsPressed));// C
  }    
}



void loop() {}

void serialEvent(){
  pinsPressed = buttonsPressed();
  /*
  Serial.println("Was r pressed?");
  Serial.println((outputMap[4].idx & pinsPressed));
  */
}


word buttonsPressed(){
  word pinsPressed = 0xFF;
  while(Serial.available()){
    incomingByte = Serial.read();
    char theChar = (char)incomingByte;
    bool valid = false;
    for(int i = 0; i < BUTTONS+1; i++){
        if(theChar == outputMap[i].key){
            pinsPressed = pinsPressed ^ outputMap[i].idx;
            valid = true;
            break;
        }
    }

    if(valid){
        Serial.println("Received");
        Serial.println(theChar);
        /*
        Serial.println("What is the code?");
        Serial.println( ? "true" : "false");*/
    }
    else{
        Serial.flush();
    }
  }
  return pinsPressed;
}
