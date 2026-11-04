#include <Arduino.h>
#include <TinyWireM.h>

// Pins of shift register 1 -> 7 Segment Display
// QH -> A
// QG -> B
// QF -> C
// QE -> D
// QD -> E
// QC -> F
// QB -> G
// QA -> DP
constexpr uint8_t dataPin = 1; // Physical Pin 6 of Attiny85  // 2 on Arduino
constexpr uint8_t shiftPin = 3; // Physical Pin 2 of Attiny85 // 3 on Arduino
constexpr uint8_t showPin = 4; // Physical Pin 3   of Attiny85  // 4 on Arduino
const uint8_t DS1307Address = 0x68;

// Number to segment mapping (common cathode)
constexpr uint8_t numbers[10] = {
  B00111111, // 0
  B00000110, // 1
  B01011011, // 2
  B01001111, // 3
  B01100110, // 4
  B01101101, // 5
  B01111101, // 6
  B00000111, // 7
  B01111111, // 8
  B01101111  // 9
};

// Digit to second register mapping
constexpr uint8_t digitMap[4] = {
  B10000000, // Digit 1 -> QH
  B01000000, // Digit 2 -> QG
  B00000001, // Digit 3 -> QA
  B00000100  // Digit 4 -> QC
};

struct Time {
  uint8_t seconds;
  uint8_t minutes;
  uint8_t hours;
};

constexpr uint8_t digitCount = 4;

// Non-blocking timers
unsigned long previousSecondMillis = 0;
const unsigned long secondInterval = 1000;
unsigned long previousDigitMillis = 0;
const unsigned long digitMultiplexInterval = 1;

int digits[4];
int currentDigit = 0;
bool dpState = false;

inline uint8_t bcdToDecimal(uint8_t binaryCodedDecimal) { return (((binaryCodedDecimal >> 4) * 10) + (binaryCodedDecimal & 0x0F)); }  
inline uint8_t decimalToBcd(uint8_t decimal) { return (((decimal / 10) << 4) | (decimal % 10)); }

void writeTime(uint8_t second, uint8_t minute, uint8_t hour, uint8_t day, uint8_t date, uint8_t month, uint8_t year) {
  TinyWireM.beginTransmission(DS1307Address);
  TinyWireM.send(0x00);
  TinyWireM.send(decimalToBcd(second));
  TinyWireM.send(decimalToBcd(minute));
  TinyWireM.send(decimalToBcd(hour));
  TinyWireM.send(decimalToBcd(day));
  TinyWireM.send(decimalToBcd(date));
  TinyWireM.send(decimalToBcd(month));
  TinyWireM.send(decimalToBcd(year));
  TinyWireM.endTransmission();
}

uint8_t getTime(uint8_t address) {
  uint8_t data = 0;
  TinyWireM.beginTransmission(DS1307Address);
  TinyWireM.send(address);
  TinyWireM.endTransmission();
  TinyWireM.requestFrom(static_cast<uint8_t>(DS1307Address), static_cast<uint8_t>(1));
  while (TinyWireM.available()) {
    data = TinyWireM.receive();
  }
  return bcdToDecimal(data);
}

void shiftOutMSB(byte value) {
  for(int i = 7; i >= 0; i--) {
    digitalWrite(shiftPin, LOW);
    int bitVal = (value >> i) & 0x01;
    digitalWrite(dataPin, bitVal);
    digitalWrite(shiftPin, HIGH);
  }
}

void shiftOutLSB(byte value) {
  for(int i = 0; i <= 7; i++) {
    digitalWrite(shiftPin, LOW);
    int bitVal = (value >> i) & 0x01;
    digitalWrite(dataPin, bitVal);
    digitalWrite(shiftPin, HIGH);
  }
}

void sendDataToShiftRegisters(byte digitByte, byte segByte) {
  digitalWrite(showPin, LOW);
  shiftOutMSB(digitByte);
  shiftOutLSB(segByte);
  digitalWrite(showPin, HIGH);
}

void resetRegisters() {
  digitalWrite(showPin, LOW);
  for(int i = 0; i < 16; i++) {
    digitalWrite(shiftPin, LOW);
    digitalWrite(dataPin, 0);
    digitalWrite(shiftPin, HIGH);
  }
  digitalWrite(showPin, HIGH);
}

void setup() {
  TinyWireM.begin();
  pinMode(dataPin, OUTPUT);
  pinMode(shiftPin, OUTPUT);
  pinMode(showPin, OUTPUT);

  //uint8_t seconds = 5;
  //uint8_t minutes = 5;
  //uint8_t hours = 16;
  //writeTime(seconds, minutes, hours, 4, 8, 10, 25);

  resetRegisters();
}

void loop() {
  unsigned long currentMillis = millis();

  // Update time every second
  if (currentMillis - previousSecondMillis >= secondInterval) {
    previousSecondMillis = currentMillis;
    uint8_t hours = getTime(0x02);
    uint8_t minutes = getTime(0x01);
    digits[0] = hours / 10; 
    digits[1] = hours % 10;
    digits[2] = minutes / 10;
    digits[3] = minutes % 10;

    dpState = !dpState;
  }

  //Multiplex 7-Segment digits
  if (currentMillis - previousDigitMillis >= digitMultiplexInterval) {
    previousDigitMillis = currentMillis;
    // Turn off all digits first
    //sendDataToShiftRegisters(0x00, 0x00);

    byte segmentByte = numbers[digits[currentDigit]];
    byte digitByte = digitMap[currentDigit];

    if (currentDigit == 1 && dpState) segmentByte |= (1 << 7);
    sendDataToShiftRegisters(digitByte, segmentByte);

    currentDigit++;
    if (currentDigit >= 4) currentDigit = 0;
  }

}
