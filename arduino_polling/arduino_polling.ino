void setup() {                
  pinMode(2, INPUT);     
  pinMode(3, OUTPUT);
}

void loop() {
  while(!digitalRead(2));
  
  digitalWrite(3, HIGH);
  delayMicroseconds(50);
  while(digitalRead(2));
  digitalWrite(3, LOW);
}
