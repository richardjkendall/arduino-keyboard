#include <CapacitiveSensor.h>
#include "CBuffer.h"

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
const int F_FREQ = 1396;
const int G_FREQ = 1567;
const int A_FREQ = 1760;
const int B_FREQ = 1975;
const int C2_FREQ = 2093;

// input key pins
const int S_PIN = 4;
const int C_KEY_PIN = 9;
const int D_KEY_PIN = 10;
const int E_KEY_PIN = 11;
const int F_KEY_PIN = 12;
const int G_KEY_PIN = 13;
const int A_KEY_PIN = 8;
const int B_KEY_PIN = 7;
const int C2_KEY_PIN = 6;

// output pins for LEDs
const int LED1_PIN = A0;
const int LED2_PIN = A1;
const int LED3_PIN = A2;
const int LED4_PIN = A3;
const int LED5_PIN = A4;
const int LED6_PIN = A5;
const int LED7_PIN = 2;
const int LED8_PIN = 3;

// output tone pin
const int TONE_PIN = 5;

// this is the threshold factor to detect a finger touching the cap sense pads
const int THRESHOLD = 5;
const int TRIGGER_COUNT = 0;
const int ABS_THRESHOLD = 2000;

// setup cap sensors
CapacitiveSensor c_key = CapacitiveSensor(S_PIN, C_KEY_PIN);        
CapacitiveSensor d_key = CapacitiveSensor(S_PIN, D_KEY_PIN);        
CapacitiveSensor e_key = CapacitiveSensor(S_PIN, E_KEY_PIN);        
CapacitiveSensor f_key = CapacitiveSensor(S_PIN, F_KEY_PIN);
CapacitiveSensor g_key = CapacitiveSensor(S_PIN, G_KEY_PIN);
CapacitiveSensor a_key = CapacitiveSensor(S_PIN, A_KEY_PIN);
CapacitiveSensor b_key = CapacitiveSensor(S_PIN, B_KEY_PIN);
CapacitiveSensor c2_key = CapacitiveSensor(S_PIN, C2_KEY_PIN);

// tone state data
int current_tone = 0;
int new_tone = 0;

// Circular buffers for smoothing
CBuffer c_buf = CBuffer();
CBuffer d_buf = CBuffer();
CBuffer e_buf = CBuffer();
CBuffer f_buf = CBuffer();
CBuffer g_buf = CBuffer();
CBuffer a_buf = CBuffer();
CBuffer b_buf = CBuffer();
CBuffer c2_buf = CBuffer();

// debug?
const bool DEBUG = false;

void setup() {
  // setup output pins
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED4_PIN, OUTPUT);
  pinMode(LED5_PIN, OUTPUT);
  pinMode(LED6_PIN, OUTPUT);
  pinMode(LED7_PIN, OUTPUT);
  pinMode(LED8_PIN, OUTPUT);

  // reset state data
  current_tone = 0;
  new_tone = 0;

  if(DEBUG) {
    Serial.begin(9600);
  }
}

void loop() {
  // read the sensors
  long c_val = c_key.capacitiveSensor(30);
  long d_val = d_key.capacitiveSensor(30);
  long e_val = e_key.capacitiveSensor(30);
  long f_val = f_key.capacitiveSensor(30);
  long g_val = g_key.capacitiveSensor(30);
  long a_val = a_key.capacitiveSensor(30);
  long b_val = b_key.capacitiveSensor(30);
  long c2_val = c2_key.capacitiveSensor(30);

  // calc current avg
  long total = c_val + d_val + e_val + f_val + g_val + a_val + b_val + c2_val;
  long avg = total / 8;

  // update cbufs
  /*
  c_buf.add(c_val);
  d_buf.add(d_val);
  e_buf.add(e_val);
  f_buf.add(f_val);
  g_buf.add(g_val);
  a_buf.add(a_val);
  b_buf.add(b_val);
  c2_buf.add(c2_val);*/

  // get moving aves
  /*
  long c_ave = c_buf.avg();
  long d_ave = d_buf.avg();
  long e_ave = e_buf.avg();
  long f_ave = f_buf.avg();
  long g_ave = g_buf.avg();
  long a_ave = a_buf.avg();
  long b_ave = b_buf.avg();
  long c2_ave = c2_buf.avg();*/

  if (DEBUG) {
    Serial.print(c_val);
    Serial.print("\t");
    Serial.print(d_val);
    Serial.print("\t");
    Serial.print(e_val);
    Serial.print("\t");
    Serial.print(f_val);
    Serial.print("\t");
    Serial.print(g_val);
    Serial.print("\t");
    Serial.print(a_val);
    Serial.print("\t");
    Serial.print(b_val);
    Serial.print("\t");
    Serial.println(c2_val);
  }

  // determine new tone value
  new_tone = 0;
  if(c_val > THRESHOLD * avg && c_val > ABS_THRESHOLD) new_tone = C_FREQ;
  if(d_val > THRESHOLD * avg && d_val > ABS_THRESHOLD) new_tone = D_FREQ;
  if(e_val > THRESHOLD * avg && e_val > ABS_THRESHOLD) new_tone = E_FREQ;
  if(f_val > THRESHOLD * avg && f_val > ABS_THRESHOLD) new_tone = F_FREQ;
  if(g_val > THRESHOLD * avg && g_val > ABS_THRESHOLD) new_tone = G_FREQ;
  if(a_val > THRESHOLD * avg && a_val > ABS_THRESHOLD) new_tone = A_FREQ;
  if(b_val > THRESHOLD * avg && b_val > ABS_THRESHOLD) new_tone = B_FREQ;
  if(c2_val > THRESHOLD * avg && c2_val > ABS_THRESHOLD) new_tone = C2_FREQ;

  // update LEDs
  digitalWrite(LED1_PIN, new_tone == C_FREQ ? HIGH : LOW);
  digitalWrite(LED2_PIN, new_tone == D_FREQ ? HIGH : LOW);
  digitalWrite(LED3_PIN, new_tone == E_FREQ ? HIGH : LOW);
  digitalWrite(LED4_PIN, new_tone == F_FREQ ? HIGH : LOW);
  digitalWrite(LED5_PIN, new_tone == G_FREQ ? HIGH : LOW);
  digitalWrite(LED6_PIN, new_tone == A_FREQ ? HIGH : LOW);
  digitalWrite(LED7_PIN, new_tone == B_FREQ ? HIGH : LOW);
  digitalWrite(LED8_PIN, new_tone == C2_FREQ ? HIGH : LOW);

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
