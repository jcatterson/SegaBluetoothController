
//
// SegaControllerSerialReader.ino
//
// Author:
//       Jon Thysell <thysell@gmail.com>
//
// Copyright (c) 2017 Jon Thysell <http://jonthysell.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

// ACM0 - 34B1F7D3C080 - slave
// ACM1 - A8108751AFD8 - master

// D8  -> Pin 4 -> DTS ->
// D10 -> Pin 3 -> TX ->
// D3 -> Pin 2 -> RX
// D4 -> Pin 1 -> DCD
// D2 -> Pin 9 -> ?
// GND ->Pin 8 -> CTS
// D11 -> Pin 7 -> RTS
// D12 -> Pin 6 -> DSR


#include "SegaController/src/SegaController.cpp"

byte pin7 = 11;
byte pin1 = 4;
byte pin2 = 3;
byte pin3 = 10;
byte pin4 = 8;
byte pin6 = 12;
byte pin9 = 2;

SegaController controller(pin7, pin1, pin2, pin3, pin4, pin6, pin9);


// Controller states
word currentState = 0;
word lastState = 0;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  currentState = controller.getState();
  sendState();
}

void sendState()
{
  // Only report controller state if it's changls ed
  if (currentState != lastState)
  {
    /*
    { pin1, 'U', 0x100},//0x100
    { pin2, 'D', 0x200},
    { pin3, 'L', 0x400},
    { pin4, 'R', 0x800},
    { pin6, 'B', 0x1000},
    { pin9, 'C', 0x2000},
    { pin6, 'A', 0x10},
    { pin9, 'S', 0x20}
     */
    word buttonsPressed = 
      ((currentState & SC_BTN_UP)    ? 0x100 : 0x0) |
      ((currentState & SC_BTN_DOWN)  ? 0x200 : 0x0)  |  
      ((currentState & SC_BTN_LEFT)  ? 0x400 : 0x0)  |
      ((currentState & SC_BTN_RIGHT) ? 0x800 : 0x0)  |
      ((currentState & SC_BTN_START) ? 0x20 : 0x0)  |
      ((currentState & SC_BTN_A)     ? 0x10 : 0x0)  |
      ((currentState & SC_BTN_B)     ? 0x1000 : 0x0)  |
      ((currentState & SC_BTN_C)     ? 0x2000 : 0x0);
      //(currentState & SC_BTN_X)     ? "X" : 0x0  |
      //(currentState & SC_BTN_Y)     ? "Y" : 0x0  |
      //(currentState & SC_BTN_Z)     ? "Z" : 0x0  |
      //(currentState & SC_BTN_MODE)  ? "M" : 0x0  |

    byte b1 = buttonsPressed & 0x00FF;
    byte b2 = (buttonsPressed & 0xFF00) >> 8;

    //Serial.println(buttonsPressed, DEC);
    Serial.print((b2 & 0xF0) >> 4, HEX);
    Serial.print((b2 & 0x0F), HEX);
    Serial.print((b1 & 0xF0) >> 4, HEX);    
    Serial.print((b1 & 0x0F), HEX);
    
    Serial.print("\n");
    //Serial.print(x);

      /*
      Serial.print((currentState & SC_CTL_ON)    ? "" : ""); // 6-BTN controller or not
      Serial.print((currentState & SC_BTN_UP)    ? "U" : "I");
      Serial.print((currentState & SC_BTN_DOWN)  ? "D" : "");    
      Serial.print((currentState & SC_BTN_LEFT)  ? "L" : "");
      Serial.print((currentState & SC_BTN_RIGHT) ? "R" : "");
      Serial.print((currentState & SC_BTN_START) ? "S" : "");
      Serial.print((currentState & SC_BTN_A)     ? "A" : "");
      Serial.print((currentState & SC_BTN_B)     ? "B" : "");
      Serial.print((currentState & SC_BTN_C)     ? "C" : "");
      Serial.print((currentState & SC_BTN_X)     ? "X" : "");
      Serial.print((currentState & SC_BTN_Y)     ? "Y" : "");
      Serial.print((currentState & SC_BTN_Z)     ? "Z" : "");
      Serial.print((currentState & SC_BTN_MODE)  ? "M" : "");
      */


    lastState = currentState;
  }
}
