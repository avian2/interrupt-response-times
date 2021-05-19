void act() {
  digitalWrite(15, HIGH);
  delayMicroseconds(50);
  digitalWrite(15, LOW);
}

void setup() {                
  pinMode(14, INPUT);     
  pinMode(15, OUTPUT);
  
  attachInterrupt(digitalPinToInterrupt(14), act, RISING);
}

void loop() {
}
