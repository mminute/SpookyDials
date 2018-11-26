class Dial {
  public:
    Dial(int pin, int lowerBound, int upperBound);
    void high();
    void low();

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

void Dial::high() {
  int high = random((_upperBound - 35), _upperBound);
  analogWrite(_pin, high);
}

void Dial::low() {
  int low = random(_lowerBound, (_lowerBound + 55));
  analogWrite(_pin, low);
}

// -- see: http://en.wikipedia.org/wiki/Fisher-Yates_shuffle
// https://forum.arduino.cc/index.php?topic=144653.0
void shuffle (Dial * t, int n) {
  while (--n >= 2) {
    // n is now the last pertinent index
    int k = random (n); // 0 <= k <= n - 1
    // Swap
    Dial temp = t [n];
    t [n] = t [k];
    t [k] = temp;
  }
}

const boolean ON = true;
const boolean OFF = false;

void set(Dial dials[], boolean onState) {
  int delay_between_dials = 10;

  shuffle(dials, 3);

  if (onState) {
    for (int i = 0; i < 3; i++) {
      dials[i].high();
      delay_between_dials = random(10, 750);
      delay(delay_between_dials);
    }
  } else {
    for (int i = 0; i < 3; i++) {
      dials[i].low();
      delay_between_dials = random(10, 750);
      delay(delay_between_dials);
    }
  }
}

//  Big MilliAmp dial
Dial dial1(9, 110, 255);
//  Small MilliAmp dial
Dial dial2(10, 70, 255);
// Amp dial- get stuck if using higher upperBound
Dial dial3(11, 0, 100);

Dial dials[3] = {dial1, dial2, dial3};

void setup() {}

void loop() {
  //  TODO: Read in the value from a range finder
  //  and set the delays based on this value
  //  maybe also set a current may analogWrite based on range finder
  set(dials, ON);
  delay(random(50, 501));
  set(dials, OFF);
  delay(random(50, 501));
}
