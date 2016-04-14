void act() {
  digitalWrite(3, HIGH);
  delayMicroseconds(50);
  digitalWrite(3, LOW);
}

void setup() {                
  pinMode(2, INPUT);     
  pinMode(3, OUTPUT);
  
  attachInterrupt(0, act, RISING);
}

void loop() {
}
