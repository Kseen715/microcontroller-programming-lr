void setup() {
  pinMode(A0, INPUT);
  Serial.begin(2400);
}

void loop() {
  int input = analogRead(A0);

  int value = map(input, 0, 1023, 0, 14);
  Serial.println(value);
}





void setup() {
  pinMode(13, OUTPUT);
}

void loop() {
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, HIGH);
  delay(1000);
}





void setup() {
  pinMode(10, OUTPUT);
}

void loop() {
  sise_t i;
  for (i = 0; i <= 150; i += 10)(
    analogWrite(10, i);
    delay(650);
  ) 
  while (i > 30) {
    i -= 10;
    analogWrite(10, i);
    delay(650);
  }
}
