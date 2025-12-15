/*
Project: Predictive Maintenance System for Rotating Machinery
Author: Ved Devendra Kale
Description:
- Temperature trend analysis
- Vibration anomaly detection
- Sensor fusion
- Trend memory with confidence scoring
- Preventive shutdown & diagnostics
*/

#include <LiquidCrystal.h>

// ---------- Trend Memory & Confidence ----------
#define WINDOW_SIZE 5
int stateHistory[WINDOW_SIZE] = {0, 0, 0, 0, 0};
int weights[WINDOW_SIZE] = {1, 1, 2, 2, 3}; // recent samples matter more

int tempPin = A0;
int motorPin = 9;

// RGB LED
int redPin = 2;
int greenPin = 3;
int bluePin = 4;

// Buzzer
int buzzer = 5;

// LCD: RS, EN, D4, D5, D6, D7
LiquidCrystal lcd(6, 7, 10, 11, 12, 13);

float currentTemp = 0;
float previousTemp = 0;
float deltaTemp = 0;

// ---------- Helper Function ----------
void setColor(bool r, bool g, bool b) {
  digitalWrite(redPin, r);
  digitalWrite(greenPin, g);
  digitalWrite(bluePin, b);
}
// -----------------------------------

void updateHistory(int newState) {
  for (int i = 0; i < WINDOW_SIZE - 1; i++) {
    stateHistory[i] = stateHistory[i + 1];
  }
  stateHistory[WINDOW_SIZE - 1] = newState;
}

int calculateConfidence(int targetState) {
  int score = 0;
  int maxScore = 0;

  for (int i = 0; i < WINDOW_SIZE; i++) {
    maxScore += weights[i];
    if (stateHistory[i] == targetState) {
      score += weights[i];
    }
  }
  return (score * 100) / maxScore;
}

void setup() {
  Serial.begin(9600);

  pinMode(motorPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(buzzer, OUTPUT);

  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Predictive");
  lcd.setCursor(0, 1);
  lcd.print("Maintenance");

  delay(2000);
  lcd.clear();
}

// Vibration analysis variables
int vibPin = A1;
int vibration = 0;

float baseline = 0;
float deviation = 0;

int sampleCount = 0;
const int baselineSamples = 5;

void loop() {

  /* ---------- TEMPERATURE TREND ---------- */
  int rawTemp = analogRead(A0);
  float temp = map(rawTemp, 1023, 0, 20, 100);
  float dT = temp - previousTemp;

  /* ---------- VIBRATION SIGNAL ---------- */
  static int vibSamples = 0;
  static float vibBaseline = 0;

  int vibRaw = analogRead(A1);
  int vibration = map(vibRaw, 1023, 0, 0, 100);

  // Baseline learning phase
  if (vibSamples < 5) {
    vibBaseline += vibration;
    vibSamples++;

    lcd.setCursor(0, 0);
    lcd.print("Learning Vib   ");
    lcd.setCursor(0, 1);
    lcd.print("Baseline...    ");

    if (vibSamples == 5) {
      vibBaseline /= 5;
    }

    previousTemp = temp;
    delay(1000);
    return;
  }

  float vibDeviation = abs(vibration - vibBaseline);

  /* ---------- VIBRATION STATE ---------- */
  bool vibAbnormal = (vibDeviation >= 15); // moderate or severe

  /* ---------- SENSOR FUSION (STATE ID) ---------- */
  int currentState;

  if (dT < 0.5 && !vibAbnormal) {
    currentState = 0; // HEALTHY
  }
  else if (dT >= 0.5 && !vibAbnormal) {
    currentState = 1; // THERMAL WEAR
  }
  else if (dT < 0.5 && vibAbnormal) {
    currentState = 2; // MECH FAULT
  }
  else {
    currentState = 3; // BEARING WEAR
  }

  /* ---------- TREND MEMORY UPDATE ---------- */
  updateHistory(currentState);
  int confidence = calculateConfidence(currentState);

  /* ---------- ACTUATION + DISPLAY ---------- */
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print((int)temp);
  lcd.print(" V:");
  lcd.print(vibration);
  lcd.print("   ");

  lcd.setCursor(0, 1);

  digitalWrite(buzzer, LOW);

  if (currentState == 0) {
    lcd.print("HEALTHY        ");
    setColor(LOW, HIGH, LOW);
    digitalWrite(motorPin, HIGH);
  }
  else if (currentState == 1) {
    lcd.print("THERM ");
    lcd.print(confidence);
    lcd.print("%   ");
    setColor(HIGH, HIGH, LOW);
    digitalWrite(motorPin, HIGH);
  }
  else if (currentState == 2) {
    lcd.print("MECH ");
    lcd.print(confidence);
    lcd.print("%    ");
    setColor(HIGH, LOW, LOW);
    digitalWrite(motorPin, HIGH);
  }
  else { // BEARING WEAR
    lcd.print("BEARING ");
    lcd.print(confidence);
    lcd.print("%");

    if (confidence >= 70) {
      setColor(HIGH, LOW, HIGH);
      digitalWrite(motorPin, LOW);
      digitalWrite(buzzer, HIGH);
    } else {
      setColor(HIGH, LOW, LOW);
      digitalWrite(motorPin, HIGH);
    }
  }

  /* ---------- SERIAL DEBUG ---------- */
  Serial.print("Temp=");
  Serial.print(temp);
  Serial.print(" dT=");
  Serial.print(dT);
  Serial.print(" Vib=");
  Serial.print(vibration);
  Serial.print(" Dev=");
  Serial.print(vibDeviation);
  Serial.print(" State=");
  Serial.print(currentState);
  Serial.print(" Conf=");
  Serial.print(confidence);
  Serial.println("%");

  previousTemp = temp;
  delay(2000);
}
