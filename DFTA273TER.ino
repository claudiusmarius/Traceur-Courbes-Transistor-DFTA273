// =========================================
// Claude DUFOURMONT
// Curve Tracer V3
// XIAO SAMD21
// Rampe DAC
//
// Mode 1 : IC = f(VCE)
// D9 = HIGH
// D10 = LOW
//
// Mode 2 : IC = f(IB)
// D9 = LOW
// D10 = HIGH
//
// Sélection par bouton sensitif D7
// Buzzer D8
// =========================================

const int DAC_PIN = A0;

// 6 résistances de sélection IB
const int IB_PINS[6] = {1, 2, 3, 4, 5, 6};

// Nouvelles E/S
const int TOUCH_PIN  = 7;
const int BUZZER_PIN = 8;

const int IN1_PIN = 9;
const int IN2_PIN = 10;

// ---------------- PARAMETRES ----------------

const int dacPoints = 1023;

//const float rampFrequency = 300.0;
const float rampFrequency = 600.0;

// --------------------------------------------

bool modeVCE = true;     // mode par défaut

// ============================================

void setup() {

  analogWriteResolution(10);

  // sorties IB
  for (int i = 0; i < 6; i++) {
    pinMode(IB_PINS[i], OUTPUT);
    digitalWrite(IB_PINS[i], LOW);
  }

  analogWrite(DAC_PIN, 0);

  pinMode(TOUCH_PIN, INPUT_PULLUP);

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  delay(100);

  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);

  // Séquence démarrage

  buzzer(1, 80, 80);
  delay(150);

  buzzer(2, 80, 80);
  delay(150);

  buzzer(3, 80, 80);
  delay(300);

  // Mode par défaut

  setMode(true);
}

// ============================================

void loop() {

  float rampPeriod_us = 1000000.0 / rampFrequency;

  float pointDelay = rampPeriod_us / dacPoints;

  for (int step = 0; step < 6; step++) {

    setIB(step);

    sweepVCE(pointDelay);

    resetIB();
  }

  // Test bouton uniquement en fin de cycle

  checkModeButton();
}

// ============================================

void sweepVCE(float pointDelay) {

  for (int i = 0; i < dacPoints; i++) {

    int dacValue = (1023 * i) / (dacPoints - 1);

    analogWrite(DAC_PIN, dacValue);

    delayMicroseconds(pointDelay);
  }

  analogWrite(DAC_PIN, 0);
}

// ============================================

void setIB(int index) {

  for (int i = 0; i < 6; i++) {
    digitalWrite(IB_PINS[i], LOW);
  }

  delayMicroseconds(200);

  digitalWrite(IB_PINS[index], HIGH);
}

// ============================================

void resetIB() {

  for (int i = 0; i < 6; i++) {
    digitalWrite(IB_PINS[i], LOW);
  }
}

// ============================================

void checkModeButton() {

  if (digitalRead(TOUCH_PIN) == LOW) {

    delay(100);

    if (digitalRead(TOUCH_PIN) == LOW) {

      modeVCE = !modeVCE;

      setMode(modeVCE);

      while (digitalRead(TOUCH_PIN) == LOW) {
        delay(10);
      }
    }
  }
}

// ============================================

void setMode(bool vceMode) {

  modeVCE = vceMode;

  if (modeVCE) {

    digitalWrite(IN1_PIN, HIGH);
    digitalWrite(IN2_PIN, LOW);

    buzzer(1, 100, 100);

  } else {

    digitalWrite(IN1_PIN, LOW);
    digitalWrite(IN2_PIN, HIGH);

    buzzer(2, 100, 100);
  }
}

// ============================================

void buzzer(int nbBips, int onTime, int offTime) {

  for (int i = 0; i < nbBips; i++) {

    digitalWrite(BUZZER_PIN, HIGH);
    delay(onTime);

    digitalWrite(BUZZER_PIN, LOW);

    if (i < nbBips - 1) {
      delay(offTime);
    }
  }
}