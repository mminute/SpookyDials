// ===========================================================================================
// ===========================================================================================
// CONSTANTS
const boolean ON = true;
const boolean OFF = false;
const int NUMBER_OF_DIALS = 3;
// Pins ======================================
const int BIG_MILLIAMP_DIAL = 9;
const int SMALL_MILLIAMP_DIAL = 10;
const int AMP_DIAL = 11;
const int SONAR_OUTPUT = A0; // Analog In Pin 0
// ===========================================================================================
// ===========================================================================================
// UTILS
int make_integer_product(float multiplier, int multiplicand) {
  return int(multiplier * multiplicand + 0.5);
}
// ===========================================================================================
// ===========================================================================================
// A Single Dial
// pin: The Arduino pin connected to the positive lead of the dial
// lowerBound: The lowest analogWrite value to be sent when a dial is to Dial.low()
// upperBound: The highest analogWrite value to be sent when a dial is to Dial.high()

class Dial {
  public:
    Dial(int pin, int lowerBound, int upperBound);
    void high(float distance_multiplier);
    void low(float distance_multiplier);

  private:
    int _pin;
    int _lowerBound;
    int _upperBound;
};


Dial::Dial(int pin, int lowerBound, int upperBound) {
  pinMode(pin, OUTPUT);
  _pin = pin;
  _lowerBound = lowerBound;
  _upperBound = upperBound;
}

// TODO: ADJUST MAX AMPLITUDE PER MULTIPLIER?
void Dial::high(float distance_multiplier) {
  int lower_bound_modifier = make_integer_product(distance_multiplier, 35);
  int high = random((_upperBound - lower_bound_modifier), _upperBound);
  analogWrite(_pin, high);
}

void Dial::low(float distance_multiplier) {
  int upper_bound_modifier = make_integer_product(1 - distance_multiplier, 55);
  int low = random(_lowerBound, (_lowerBound + upper_bound_modifier));
  analogWrite(_pin, low);
}
// ===========================================================================================
// ===========================================================================================
// A collection of dials
// all_dials: The dials in the collection
class Dials {
  public:
    Dials(Dial all_dials[]);
    void set(boolean onState, float multiplier);

  private:
    Dial* _dials;
    void shuffle(Dial * t, int);
};

Dials::Dials(Dial dial_set[]) {
  _dials = dial_set;
}

void Dials::shuffle(Dial * t, int n) {
  // -- see: http://en.wikipedia.org/wiki/Fisher-Yates_shuffle
  // https://forum.arduino.cc/index.php?topic=144653.0
  while (--n >= 2) {
    // n is now the last pertinent index
    int k = random (n); // 0 <= k <= n - 1
    // Swap
    Dial temp = t [n];
    t [n] = t [k];
    t [k] = temp;
  }
}

void Dials::set(boolean onState, float multiplier) {
  shuffle(_dials, NUMBER_OF_DIALS);
  int upper_limit = multiplier * 750;

  if (onState) {
    for (int i = 0; i < 3; i++) {
      _dials[i].high(multiplier);
      delay(random(10, upper_limit));
    }
  } else {
    for (int i = 0; i < 3; i++) {
      _dials[i].low(multiplier);
      delay(random(10, upper_limit));
    }
  }
}
// ===========================================================================================
// ===========================================================================================
float distanceMultiplier(int input_value) {
  int distance;
  // value below is the freak out point.  Adjust this to arms length.
  if (input_value <= 15) {
    distance = 1;
  } else {
    distance = min(input_value, 180);
  }

  return distance/180.0;
}

int getDelay(float multiplier) {
  int upperLimit = make_integer_product(multiplier, 501);
  return random(50, upperLimit);
}
// ===========================================================================================
// ===========================================================================================
// RUN

// Define the dials and their properties
Dial dial1(BIG_MILLIAMP_DIAL, 110, 255);
Dial dial2(SMALL_MILLIAMP_DIAL, 70, 255);
Dial dial3(AMP_DIAL, 0, 100);

// Collect the dials
Dial dial_set[NUMBER_OF_DIALS] = {dial1, dial2, dial3};

// Create the set of dials to be operated upon
Dials dials(dial_set);

void setup() {}

void loop() {
  float multiplier = distanceMultiplier(analogRead(SONAR_OUTPUT));

  dials.set(ON, multiplier);
  delay(getDelay(multiplier));
  dials.set(OFF, multiplier);
  delay(getDelay(multiplier));
}
