int relay = 5;//d1
volatile byte relayState = LOW;
long lastDebounceTime = 0;  
long debounceDelay = 10000;
void setup() {
  pinMode(relay, OUTPUT);
}

void loop() {
  digitalWrite(relay, HIGH);
  delay(1000);
  digitalWrite(relay,LOW);
}
