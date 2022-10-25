/* Arduino IDE 1.8.15 */

#define PIN_POT   A3
#define PIN_DIR   A1
#define OUT_CW    5
#define OUT_CCW   6

//#define DEBUG

void setup(void)
{
  pinMode(PIN_POT, INPUT); 
  pinMode(PIN_DIR, INPUT); 
  pinMode(OUT_CW, OUTPUT);
  pinMode(OUT_CCW, OUTPUT);

  //https://www.etechnophiles.com/how-to-change-the-pwm-frequency-of-arduino-nano/
  TCCR0B = TCCR0B & B11111000 | B00000010; // for PWM frequency of 7812.50 Hz

#ifdef DEBUG
  Serial.begin(115200);
#endif //DEBUG
}

void loop(void)
{
  /* reversing the value */
  long in_speed = 1023 - analogRead(PIN_POT);
  /* scale 0-1023 => 0-255 */
  in_speed = (in_speed * 255) / 1023;

  /* Activating the motor */
  if(digitalRead(PIN_DIR))
  {
    analogWrite(OUT_CW, 0);
    analogWrite(OUT_CCW, in_speed);
  }
  else
  {
    analogWrite(OUT_CCW, 0);
    analogWrite(OUT_CW, in_speed);
  }

#ifdef DEBUG
  Serial.println(in_speed);
#endif //DEBUG
}
