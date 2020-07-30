#include <elapsedMillis.h>
#include <MIDIUSB.h>
#include <frequencyToNote.h>
#include <MIDIUSB_Defs.h>
#include <pitchToFrequency.h>
#include <pitchToNote.h>


#define NUM_BUTTONS  7

/* USB MIDI AnalogControlChange Example

   You must select MIDI from the "Tools > USB Type" menu
   http://www.pjrc.com/teensy/td_midi.html

   This example code is in the public domain.
*/

//#include <Bounce.h>

// MIDI channel number is critical and different for every microcontroller
//use http://jsfiddle.net/heinzotto/qoe4sfaj/4/ for debugging and midi monitoring
const int channel = 0;


//Calibration of Midi controller, array with desired values and array with their corresponding analogReads
//temporarily mask midicontroller with tape and measure distances across whoel range
//e.g. 64 is middle of sliding range and corresponding analog value is 823

//>>> numpy.linspace(2,1022, 16)
//array([   2.,   70.,  138.,  206.,  274.,  342.,  410.,  478.,  546.,
//        614.,  682.,  750.,  818.,  886.,  954., 1022.])

        
//SLIDER 1
//wanted
int out0[] = {0, 8, 16, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120, 128};
//correspnding analogread values
int in0[]  = { 2, 70, 138, 206, 274, 342, 410, 478, 546, 614, 682, 750, 818, 886, 954, 1022};


//SLIDER 2
//wanted
int out1[] = {0, 8, 16, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120, 128};
//correspnding analogread values
int in1[]  = { 2, 70, 138, 206, 274, 342, 410, 478, 546, 614, 682, 750, 818, 886, 954, 1022};             

/*
//SLIDER 3
//wanted
int out2[] = {0, 8, 16,   32,   40, 48,  56,  64,  72,  80,  88,   96, 104, 112, 120, 128};
//correspnding analogread values
int in2[]  = { 2, 70, 138, 206, 274, 342, 410, 478, 546, 614, 682, 750, 818, 886, 954, 1022};             //nonlinear_remap, stuttering at end and beginning {3, 5, 13, 35, 90, 206, 342, 478, 628, 767, 914, 963, 1000, 1010, 1016, 1022}; 
               
*/

// the MIDI continuous controller for each analog input
const int controllerA0 = 2; // 10 = pan position
const int controllerA1 = 3; // 11 = volume/expression
//const int controllerA2 = 4; // 91 = reverb level

const int button1 = 2;
const int button2 = 3;
const int button3 = 4;
const int button4 = 5;
const int button5 = 6;
const int button6 = 7;
const int button7 = 8;

const int intensityPot = 5;



const uint8_t buttons[NUM_BUTTONS] = {button1, button2, button3, button4, button5, button6, button7};
//const byte notePitches[NUM_BUTTONS] = {C3, D3, E3, F3, G3, A3, B3};

uint8_t notesTime[NUM_BUTTONS];
uint8_t pressedButtons = 0x00;
uint8_t previousButtons = 0x00;
uint8_t intensity;



void setup() {
  for (int i = 0; i < NUM_BUTTONS; i++)
    pinMode(buttons[i], INPUT_PULLUP);  
  Serial.begin(115200);
}

// store previously sent values, to detect changes
int previousA0 = -1;
int previousA1 = -1;
//int previousA2 = -1;




elapsedMillis msec = 0;

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

void loop() {
  // only check the analog inputs 50 times per second,
  // to prevent a flood of MIDI messages
  if (msec >= 20) {
    msec = 0;
    int n0 = analogRead(A0) ;
    int n1 = analogRead(A1) ;
 //   int n2 = analogRead(A2) ;
    


    int n0_linear = multiMap(n0, in0, out0, 16);
    int n1_linear = multiMap(n1, in1, out1, 16);
 //   int n2_linear = multiMap(n2, in2, out2, 16);



  readButtons();
  readIntensity();

//SLIDER 1
    // only transmit MIDI messages if analog input changed
    if (n0 - 1 > previousA0 || n0 + 1 < previousA0) {

      //channel, control, value;
      controlChange(0,controllerA0, n0_linear);
      MidiUSB.flush();
      previousA0 = n0;
     Serial.println(analogRead(A0));
     Serial.println(n0_linear);
     //Serial.println(n0_linear);
    }

    


//SLIDER 2
    // only transmit MIDI messages if analog input changed
    if (n1 - 1 > previousA1 || n1 + 1 < previousA1) {

      //channel, control, value;
      controlChange(0,controllerA1, n1_linear);
      MidiUSB.flush();
      previousA1 = n1;
     Serial.println(analogRead(A1));
     //Serial.println(n1_linear);
    }

    


/*
//SLIDER 3
    // only transmit MIDI messages if analog input changed
    if (n2 - 1 > previousA2 || n2 + 1 < previousA2) {

      //channel, control, value;
      controlChange(0,controllerA2, n2_linear);
      MidiUSB.flush();
      previousA2 = n2;
     Serial.println(analogRead(A2));
     //Serial.println(n2_linear);
    }


*/

    

  } //every 20ms


} //loop


void readButtons()
{
  for (int i = 0; i < NUM_BUTTONS; i++)
  {
    if (digitalRead(buttons[i]) == LOW)
    {
      bitWrite(pressedButtons, i, 1);
      delay(50);
    }
    else
      bitWrite(pressedButtons, i, 0);
  }
}

void readIntensity()
{
  int val = analogRead(intensityPot);
  intensity = (uint8_t) (map(val, 0, 1023, 0, 127));
}



int multiMap(int val, int* _in, int* _out, uint8_t size)
{
  // take care the value is within range
  // val = constrain(val, _in[0], _in[size-1]);
  if (val <= _in[0]) return _out[0];
  if (val >= _in[size - 1]) return _out[size - 1];

  // search right interval
  uint8_t pos = 1;  // _in[0] allready tested
  while (val > _in[pos]) pos++;

  // this will handle all exact "points" in the _in array
  if (val == _in[pos]) return _out[pos];

  // interpolate in the right segment for the rest
  return (val - _in[pos - 1]) * (_out[pos] - _out[pos - 1]) / (_in[pos] - _in[pos - 1]) + _out[pos - 1];
}
