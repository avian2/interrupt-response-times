#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))

void act() {
  digitalWrite(3, HIGH);
  delayMicroseconds(50);
  digitalWrite(3, LOW);
}

void setup() {                
  pinMode(2, INPUT);     
  pinMode(3, OUTPUT);

  cbi(TIMSK0, TOIE0);
  
  attachInterrupt(0, act, RISING);
}

void loop() {
}
