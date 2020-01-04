
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
        Serial.print((currentState & SC_CTL_ON)    ? "" : "-");
        Serial.print((currentState & SC_BTN_UP)    ? "U" : "");
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

        Serial.print("\n");
        lastState = currentState;
    }
}
