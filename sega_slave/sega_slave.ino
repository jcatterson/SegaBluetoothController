byte pin7 = 2;
byte pin1 = 9;
byte pin2 = 5;
byte pin3 = 4;//L
byte pin4 = 7;
byte pin6 = 3;//A
byte pin9 = 6;

int incomingByte = 0;
int counter = 0;
word pinsPressed = 0x0;//FFFF;
word lastBtnsPressed;
int numTimesThrough = 0;

typedef struct
{
    byte pin;
    char key;
    word idx;
} output;



output outputMap[] = {
    { pin1, 'U', 0x100},//0x100
    { pin2, 'D', 0x200},
    { pin3, 'L', 0x400},
    { pin4, 'R', 0x800},
    { pin6, 'B', 0x1000},
    { pin9, 'C', 0x2000},
    { pin6, 'A', 0x10},
    { pin9, 'S', 0x20}
};

const byte BUTTONS = 8;

void loop() {}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(pin1, OUTPUT);
  pinMode(pin7, INPUT);
  pinMode(pin9, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);
  pinMode(pin6, OUTPUT);
  pinMode(pin9, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(pin7), readValue, CHANGE);
  word initState = 0xFFFF;//0x0000;//0xFFDF;
  //initController(initState);
  lastBtnsPressed = initState;
}

void cyclePins(){
  for(int i = 0; i < 4; i++){
    writeController(i % 2 == 0);
  }
  lastBtnsPressed = 0xFFFF;
}

void readValue(){
  int selectPin = digitalRead(pin7);  
  writeController(selectPin);
}

byte cycle = 0;
void writeController(int selectPin){
  if(selectPin == 0){   
    writeLow();
  }
  else if(selectPin == 1){
    writeHigh();
  }
}


void writeLow(){
  word lastBtnsPressed = pinsPressed;

  byte bitValue = (0x1 & (lastBtnsPressed >> 8));// U


  byte a = ((lastBtnsPressed >> 4) & 0x1) << 3;
  byte s = ((lastBtnsPressed >> 5) & 0x1) << 6;

  byte l = 0x1 << 4;
  byte r = 0x1 << 7;

  byte ctrlOn = l | r;
  
  PORTD = ( PORTD & 0x7 ) | ~(ctrlOn |  a | s);
  PORTB = (PORTB ^ 0x2) | (bitValue << 1);

  //Clear Approach
  //pinsPressed = pinsPressed & 0xFF00;

}

void writeHigh(){ 
  word lastBtnsPressed = pinsPressed;
  
  // Up is not working 2/9/2020
  byte bitValue = ~(0x1 & (0x1 & (lastBtnsPressed >> 8)));// U
  

  byte d = ((lastBtnsPressed >> 9)& 0x1) << 5;
  byte l = ((lastBtnsPressed >> 10)& 0x1) << 4;
  byte r = ((lastBtnsPressed >> 11)& 0x1) << 7;
  byte c = ((lastBtnsPressed >> 13) & 0x1) << 6;
  byte b = ((lastBtnsPressed >> 12) & 0x1) << 3;
  
  byte port = ~(d | l | r | b | c);
  
  byte toWrite = (PORTD & 0x7) | port;
  PORTD = toWrite;
  PORTB = (PORTB ^ 0x2) | (bitValue << 1);

  //Clear Approach
  //pinsPressed = pinsPressed & 0x00FF;
}

void initController(word pinCode){
  //lastBtnsPressed = ~pinCode;
  //Serial.println(lastBtnsPressed);

  byte bitValue = 0x0;//(0x1 & (lastBtnsPressed >> 8));// U
  PORTB = (PORTB ^ 0x2) | (bitValue << 1);

  byte a = ((lastBtnsPressed >> 4) & 0x1) << 3;
  byte s = ((lastBtnsPressed >> 5) & 0x1) << 6;
  byte d = ((lastBtnsPressed >> 9)& 0x1) << 5;
  byte l = ((lastBtnsPressed >> 10)& 0x1) << 4;
  byte r = ((lastBtnsPressed >> 11)& 0x1) << 7;
  byte c = ((lastBtnsPressed >> 13) & 0x1) << 6;
  byte b = ((lastBtnsPressed >> 12) & 0x1) << 3;
  
  
  byte port = ~(a | s | d | l | r | b | c | 0x7);
  
  byte toWrite = (PORTD & 0x7) | port;

  PORTD = toWrite;  
  
}

void serialEvent(){
  char bits[4] = "0000";
  Serial.readBytesUntil('\n', bits, 5);
  char * pEnd;

  pinsPressed = strtol(bits, &pEnd, 16);
}
