#include <CapacitiveSensor.h>

/*
 * Arduino Keyboard Sketch
 * Richard Kendall 2020
 * Uses capacitive sensing pads to play tones, as part of a childrens toy
 * 
 * More details here
 * https://github.com/richardjkendall/arduino-keyboard
 */

// tone freqs
const int C_FREQ = 1046;
const int D_FREQ = 1175;
const int E_FREQ = 1318;

// input key pins
const int C_KEY_PIN = 2;
const int D_KEY_PIN = 5;
const int E_KEY_PIN = 8;

// output pins for LEDs
const int LED1_PIN = A0;
const int LED2_PIN = A1;
const int LED3_PIN = A2;

// output tone pin
const int TONE_PIN = 10;

// this is the threshold used to detect a finger touching the cap sense pads
const int THRESHOLD = 1000;

// setup cap sensors
CapacitiveSensor c_key = CapacitiveSensor(4, C_KEY_PIN);        // 10 megohm resistor between pins 4 & 2, pin 2 is sensor pin, add wire, foil
CapacitiveSensor d_key = CapacitiveSensor(4, D_KEY_PIN);        // 10 megohm resistor between pins 4 & 6, pin 6 is sensor pin, add wire, foil
CapacitiveSensor e_key = CapacitiveSensor(4, E_KEY_PIN);        // 10 megohm resistor between pins 4 & 8, pin 8 is sensor pin, add wire, foil

// tone state data
int current_tone = 0;
int new_tone = 0;

void setup() {
  // setup output pins
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);

  // reset state data
  current_tone = 0;
  new_tone = 0;
  
   
   //cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
  Serial.begin(9600);

}

void loop() {
  // read the sensors
  long c_val = c_key.capacitiveSensor(30);
  long d_val = d_key.capacitiveSensor(30);
  long e_val = e_key.capacitiveSensor(30);

  Serial.print(c_val);
  Serial.print("\t");
  Serial.print(d_val);
  Serial.print("\t");
  Serial.println(e_val);

  // determine new tone value
  new_tone = 0;
  if(c_val > THRESHOLD) new_tone = C_FREQ;
  if(d_val > THRESHOLD) new_tone = D_FREQ;
  if(e_val > THRESHOLD) new_tone = E_FREQ;

  // update LEDs
  digitalWrite(LED1_PIN, c_val > THRESHOLD ? HIGH : LOW);
  digitalWrite(LED2_PIN, d_val > THRESHOLD ? HIGH : LOW);
  digitalWrite(LED3_PIN, e_val > THRESHOLD ? HIGH : LOW);

  // do we need to take action?
  if(new_tone != current_tone) {
    // is new tone 0, then stop the tone
    if(new_tone == 0) {
      noTone(TONE_PIN);
    } else {
      tone(TONE_PIN, new_tone);
    }
    current_tone = new_tone;
  }
}
