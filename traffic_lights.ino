#include <millisDelay.h>

#include "SevSeg.h"
SevSeg sevseg;

// timer
millisDelay ledTimer;
int secondsLeft = 0;

// leds
const int red = 2;
const int yellow = 3;
const int green = 4;

// button
const int button = 5;
int buttonState = 0;
unsigned long lastDebounce = 0;

// colors
String nextColor = "red";
String lastColor = "red";

void setup() {
  configSegmentNumber();
  newSegmentNumber(0);
  
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);

  pinMode(button, INPUT);
}

void loop() {
  light(nextColor);
}

void light(String lightColor) {
  if (lightColor == "red") {
    lightRed();
  } else if (lightColor == "yellow") {
    lightYellow();
  } else {
    lightGreen();
  }
}

void lightRed() {
  ledTimer.start(5500);
  nextColor = "yellow";
  lastColor = "red";

  secondsLeft = ledTimer.remaining() / 1000;

  do {
    buttonState = digitalRead(button);
    bool buttonPressed = debounce(buttonState, lastDebounce, button);
    
    if (buttonPressed) {
      ledTimer.finish();
      lightGreen();
      break;
    } else {
      digitalWrite(green, LOW);
      digitalWrite(yellow, LOW);
      digitalWrite(red, HIGH);
    }

    if (ledTimer.remaining() / 1000 != secondsLeft) {
      secondsLeft = ledTimer.remaining() / 1000;
      newSegmentNumber(secondsLeft);
    }
  } while (!ledTimer.isFinished());
}

void lightYellow() {
  ledTimer.start(600);
 
  if (lastColor == "red") {
    nextColor = "green";
  } else {
    nextColor = "red";
  }

  secondsLeft = ledTimer.remaining() / 1000;
  
  do {
    buttonState = digitalRead(button);
  
    bool buttonPressed = debounce(buttonState, lastDebounce, button);
    
    if (buttonPressed) {
      ledTimer.finish();
      lightGreen();
      break;
    } else {
      digitalWrite(red, LOW);
      digitalWrite(green, LOW);
      digitalWrite(yellow, HIGH);
    }

    if (ledTimer.remaining() / 1000 != secondsLeft) {
      secondsLeft = ledTimer.remaining() / 1000;
      newSegmentNumber(secondsLeft);
    }
  } while (!ledTimer.isFinished());
}

void lightGreen() {
  ledTimer.start(5500);
  nextColor = "yellow";
  lastColor = "green";
  
  secondsLeft = ledTimer.remaining() / 1000;

  do {
    digitalWrite(red, LOW);
    digitalWrite(yellow, LOW);
    digitalWrite(green, HIGH);

    if (ledTimer.remaining() / 1000 != secondsLeft) {
      secondsLeft = ledTimer.remaining() / 1000;
      newSegmentNumber(secondsLeft);
    }
  } while (!ledTimer.isFinished());
}

void configSegmentNumber() {
  // Set digit display
  byte numDigits = 1;
  byte digitPins[] = {};
  byte segmentPins[] = {12, 11, 8, 7, 6, 13, 10, 9};
  bool resistorsOnSegments = true;

  byte hardwareConfig = COMMON_CATHODE;
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(90);
}

void newSegmentNumber(int num) {
  Serial.println(num);
  sevseg.setNumber(num);
  sevseg.refreshDisplay();
}

bool debounce(bool lastButtonState, long lastDebounce, int button){
  bool current = digitalRead(button);
  if (current != lastButtonState && millis()-lastDebounce > 50){
    current = digitalRead(button);
    lastDebounce = millis();
  }
  return current;
}
