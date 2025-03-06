#include <Servo.h>

Servo myservo;

int potpin = A0; // Potentiometer pin
int servopin = 9;
int potValue = 0;
int LEDm = 3; // Left LED (Morse Code for English)
int LEDc = 11; // Right LED (Morse Code for Chinese)

// Morse code for "WHERE ARE YOU FROM?"
const char* morseCodeLeft = ".-- .... . .-. . / .- .-. . / -.-- --- ..- / ..-. .-. --- -- ..--..";

// Morse code for "你不属于这里 (Nǐ bù shǔyú zhèlǐ)"
const char* morseCodeRight = "----- .---- ...-- ..--- / ----- ----- ----- ---.. / .---- ....- -.... -.... / ----- ----- -.... ----- / -.... -.... ...-- ---.. / -.... ---.. ....- ----.";

int baseDotDuration = 100; // Base duration for a dot
unsigned long morseMillisLeft = 0;
unsigned long morseMillisRight = 0;
int morseIndexLeft = 0;
int morseIndexRight = 0;
bool ledStateLeft = false;
bool ledStateRight = false;

const int DOT_BLNK_LEN = 500; // Length of a dot is one unit
const int DASH_BLNK_LEN = 1500; // Length of a dash is three units

void setup() {
  Serial.begin(9600);
  pinMode(potpin, INPUT);
  pinMode(LEDm, OUTPUT);
  pinMode(LEDc, OUTPUT);
  myservo.attach(servopin);

  // Turn on LEDs at the start to test wiring
  digitalWrite(LEDm, HIGH);
  digitalWrite(LEDc, HIGH);
  delay(2000); // Keep them on for 2 seconds
  digitalWrite(LEDm, LOW);
  digitalWrite(LEDc, LOW);
}

void loop() {
  unsigned long currentMillis = millis();
  
  // Read potentiometer value and map to a wider range for noticeable changes
  int potValue = analogRead(potpin);
  // Map potentiometer value to control LEDs' blinking speed
  potValue = map(potValue, 0, 1023, 0, 200);
  myservo.write(potValue);
  delay(15);

  // Calculate the inverse blinking speed for the LEDs
  int speedLeft = potValue;  
  Serial.println("left");
  Serial.println(speedLeft);
  int speedRight = 200-potValue;  // inverse
  Serial.println("right");
  Serial.println(speedRight);
  
  // Morse code blinking for Left LED (English)
  if (currentMillis - morseMillisLeft >= speedLeft)
  {
    morseMillisLeft = currentMillis;
    if (morseCodeLeft[morseIndexLeft] == '.') {
      ledStateLeft = !ledStateLeft;
      digitalWrite(LEDm, ledStateLeft ? HIGH : LOW);
    } else if (morseCodeLeft[morseIndexLeft] == '-') {
      ledStateLeft = !ledStateLeft;
      digitalWrite(LEDm, ledStateLeft ? HIGH : LOW);
    } else if (morseCodeLeft[morseIndexLeft] == ' ') {
      delay(speedLeft * 3); // Space between letters
    }
    morseIndexLeft = (morseIndexLeft + 1) % strlen(morseCodeLeft);
  }


  // Morse code blinking for Right LED (Chinese)
  if (currentMillis - morseMillisRight >= speedRight)
  {
    morseMillisRight = currentMillis;
    if (morseCodeRight[morseIndexRight] == '.') {
      ledStateRight = !ledStateRight;
      digitalWrite(LEDc, ledStateRight ? HIGH : LOW);
    } else if (morseCodeRight[morseIndexRight] == '-') {
      ledStateRight = !ledStateRight;
      digitalWrite(LEDc, ledStateRight ? HIGH : LOW);
    } else if (morseCodeRight[morseIndexRight] == ' ') {
      delay(speedRight * 3); // Space between letters
    }
    morseIndexRight = (morseIndexRight + 1) % strlen(morseCodeRight);
  }
}
