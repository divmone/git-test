#define SW 4
#define MODE_AM 6
#define LEDS 5
#define CLK 2  //s1
#define DT 3 //s2
#include <EEPROM.h>
#include <GyverEncoder.h>
#include <GyverTimers.h>
#include <FastIO.h>



volatile int colors[LEDS];
byte ledBrigth = 100;
int ledDelay = 500;
int direction = 1;
int currentLed = 0;
int fadeAmount = 5;
int bright = 0;
int i = 0;
unsigned long changeTime;
bool flag;
bool eepromFlag = false;
const uint8_t pins[] = {5, 6, 9, 10, 11};
uint8_t mode = 0;
uint32_t eepromTimer = 0;

Encoder enc1(CLK, DT, SW);

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 13; i++) {
    pinMode(pins[i], OUTPUT);
  }
  changeTime = millis();
  enc1.setType(TYPE2);
  Timer2.setFrequency(40000);
  Timer2.enableISR();
  EEPROM.get(0, mode);
  EEPROM.get(50, ledDelay);
}

void enc() {
  enc1.tick();

  if (enc1.isLeft())  {
    ledDelay = ledDelay - 50;
    if (ledDelay == 0) ledDelay = 50;
    eepromFlag = true;
    eepromTimer = millis();
  }
  if (enc1.isRight()) {
    ledDelay = ledDelay + 50;
    eepromFlag = true;
    eepromTimer = millis();
  }
  if (enc1.isClick()) {
    if (++mode >= MODE_AM) mode = 0;
    for (uint8_t i = 0; i < LEDS; i++) colors[i] = 0;
    EEPROM.put(0, mode);

  }
  /*if (enc1.isLeftH())  {
    ledBrigth = ledBrigth - 10;
    
  }
  if (enc1.isRightH()) {
    ledBrigth = ledBrigth + 10;
    

  }*/
}


void loop() {
  checkEEPROM();
  enc();
  static uint32_t timer = millis();
  if (millis() - timer >= ledDelay / 5) {
    timer = millis();
    switch (mode) {
      case 3: task_3();
        break;
      case 4: task_4();
        break;
      case 5: task_5();
        break;
    }
  }
  switch (mode) {
    case 0: task_0();
      break;
    case 1: task_1();
      break;
    case 2: task_2();
      break;

  }
}




void changeLed() {
  for (int i = 0; i < LEDS; i++) {
    digitalWrite(pins[i], LOW);
  }
  digitalWrite(pins[currentLed], ledBrigth);
  currentLed += direction;
  if (currentLed == LEDS - 1 ) {
    direction = -1;
  }
  if (currentLed == 0) {
    direction = 1;
  }
}



void changeLed2() {
  for (int i = 0; i < LEDS; i++) {
    digitalWrite(pins[i], LOW);
  }
  digitalWrite(pins[currentLed], ledBrigth);
  currentLed += direction;
  if (currentLed == LEDS) {
    currentLed = 0;
  }
  if (currentLed == 0) {
    direction = 1;
  }
}
void changeLed3() {
  static byte i = 0;
  digitalWrite(pins[i], ledBrigth);
  i++;
  if (i > LEDS) {
    for (int i = 0; i < LEDS; i++) {
      digitalWrite(pins[i], LOW);
    }
    i = 0;
  }
}


void fade() {
  bright = bright + fadeAmount;
  if (bright <= 0 || bright >= 255) {
    fadeAmount = -fadeAmount;
  }
  for (int i = 0; i < LEDS; i++) {
    colors[i] = bright;
  }
}



void confetti() {
  for (int i = 0; i < LEDS; i++) {
    if (colors[i] == 0 && ((int)random(0, 15) == 0) ) {
      colors[i] = 255;
    }
    colors[i] = max(colors[i] -= 10, 0);
  }
}

int trackPos = 0;
int trackVel = 4;
void track() {
  trackPos += trackVel;
  if (trackPos < 0 || trackPos >= LEDS * 10) trackVel = -trackVel;
  colors[constrain(trackPos / 10, 0, LEDS - 1)] = 255;
  for (int i = 0; i < LEDS; i++) {
    colors[i] = max(colors[i] -= 15, 0);
  }
}

void task_0() {
  if ((millis() - changeTime) > ledDelay) {
    changeLed();
    changeTime = millis();
  }
}
void task_1() {
  if ((millis() - changeTime) > ledDelay) {
    changeLed2();
    changeTime = millis();
  }
}
void task_2() {
  if ((millis() - changeTime) > ledDelay) {
    changeLed3();
    changeTime = millis();
  }
}

void task_3() {
  fade();
}

void task_4() {
  confetti();
}

void task_5() {
  track();
}


void pwmTick() {
  static volatile uint8_t counter = 0;
  if (counter == 0) {
    for (uint8_t i = 0; i < LEDS; i++) {
      if (colors[i] > 0) fastWrite(pins[i], ledBrigth);
    }
  }
  for (uint8_t i = 0; i < LEDS; i++) {
    if (counter == colors[i] && mode > 2 ) {
      fastWrite(pins[i], LOW);
    }
  }
  counter++;
}

ISR(TIMER2_A) {
  pwmTick();
}



void checkEEPROM() {
  if (eepromFlag && (millis() - eepromTimer >= 10000) ) {
    eepromFlag = false;
    EEPROM.put(1, ledDelay);
  }
}
