byte pin7 = 2;//7;
byte pin1 = 9;
byte pin2 = 1;
byte pin3 = 4;
byte pin4 = 7;//2;
byte pin6 = 3;
byte pin9 = 6;

int incomingByte = 0;
int counter = 0;
word pinsPressed = 0xFFFF;
word lastBtnsPressed;

typedef struct
{
    byte pin;
    char key;
    word idx;
} output;


output outputMap[] = {
    { pin1, 'U', 0x100},
    { pin2, 'D', 0x200},
    { pin3, 'L', 0x400},
    { pin4, 'R', 0x800},
    { pin6, 'B', 0x10},
    { pin9, 'C', 0x2000},
    { pin6, 'A', 0x100},
    { pin9, 'S', 0x200},
};

const byte BUTTONS = 8;


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
  attachInterrupt(digitalPinToInterrupt(pin7), writeController, CHANGE);
  initController(0xEF);
  Serial.println("Done with setup");
}

byte cycle = 0;
void writeController(){
  int selectPin = digitalRead(pin7);
  
  if(pinsPressed != 0xFFFF){
    cycle++;

    if(selectPin == HIGH){
      //Serial.println("What is the bit?");
      //Serial.println(pinsPressed);
      for(int i = 0; i < 6; i++){
        int bitShift = 8 + i;
        if((pinsPressed >> bitShift) != (lastBtnsPressed >> bitShift) ){
          Serial.println(outputMap[i].key);
          bool result = (bool)outputMap[i].idx & pinsPressed;
          Serial.println(result);
          digitalWrite(outputMap[i].pin, result);
        }        
      }
      lastBtnsPressed = pinsPressed;
    }
    else {
      digitalWrite(pin9, HIGH);
    }

    

    if(cycle == 60){
      Serial.println("clear");
      initController(0xFF);
      cycle = 0;
    }
  }

}

void initController(word pinCode){
  lastBtnsPressed = pinCode;
  for(int i = 0; i < 6; i++){
    byte pinStatus = (pinCode >> i) & 1;
    digitalWrite(outputMap[i].pin, pinStatus);
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
  word pinsPressed = 0xFFFF;
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
