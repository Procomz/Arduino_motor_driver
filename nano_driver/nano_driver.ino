/* Arduino IDE 1.8.15 */

#define PIN_POT   A3
#define PIN_DIR   A1
#define OUT_CW    5
#define OUT_CCW   6

//#define DEBUG

int cpt = 0;

long in_speed = 0;
long command = 0;

void setup(void)
{
#ifdef DEBUG
  Serial.begin(115200);
#endif //DEBUG

  pinMode(PIN_POT, INPUT); 
  pinMode(PIN_DIR, INPUT); 
  pinMode(OUT_CW, OUTPUT);
  pinMode(OUT_CCW, OUTPUT);

  //https://www.etechnophiles.com/how-to-change-the-pwm-frequency-of-arduino-nano/
  TCCR0B = TCCR0B & B11111000 | B00000010; // for PWM frequency of 7812.50 Hz

  /* Timer 1 interrupt init */
  /* https://batchloaf.wordpress.com/2022/04/04/arduino-nano-timer-1-interrupt-example-for-100-hz-sampling/ */
  cli(); // disable interrupts during setup
  // F_clock = 16MHz, prescaler = 64, Fs = 100 Hz
  TCCR1A = 0;
  TCCR1B = 1<<WGM12 | 0<<CS12 | 1<<CS11 | 1<<CS10;
  TCNT1 = 0;          // reset Timer 1 counter
  // OCR1A = ((F_clock / prescaler) / Fs) - 1 = 2499
  OCR1A = 2499;       // Set sampling frequency Fs = 100 Hz
  TIMSK1 = 1<<OCIE1A; // Enable Timer 1 interrupt
  sei(); // re-enable interrupts
}

void loop(void)
{
  /* reversing the value */
  in_speed = 1023 - analogRead(PIN_POT);
  /* scale 0-1023 => 0-255 */
  in_speed = (in_speed * 255) / 1023;

  /* signing the speed */
  if(digitalRead(PIN_DIR))
  {
    in_speed = - in_speed;
  }
  
  /* Activating the motor */
  if(command > 0)
  {
    analogWrite(OUT_CW, 0);
    analogWrite(OUT_CCW, abs(command));
  }
  else
  {
    analogWrite(OUT_CCW, 0);
    analogWrite(OUT_CW, abs(command));
  }

#ifdef DEBUG
  //Serial.println(in_speed);
#endif //DEBUG
}

/* Timer 1 interrupt every 10ms */
ISR(TIMER1_COMPA_vect)
{
  /* Only incrementing 1 every 10ms */
  if(in_speed > command)
  {
    command ++;
  }
  else if(in_speed < command)
  {
    command --;
  }
  /* if command = in_speed, no need to change */
  
  
#ifdef DEBUG
  cpt ++;
  if(cpt >= 100)
  {
    cpt = 0;
    /* divided by 8 because millis is impacted by TCCR0B */
    //Serial.println(millis()/8);
  }
  Serial.println(command);
#endif //DEBUG
}
