
void setup() {
  // initialize serial:
  Serial.begin(9600);
  pinMode(4, OUTPUT);
}

void loop() {
}


void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    
   // char inChar = (char)Serial.read();
    int inInt = Serial.parseInt();
    //inputString += inChar;
    if (inInt == 295) {
      digitalWrite(4, HIGH);
      delay(100);
      digitalWrite(4, LOW);
      delay(100);
    }
  }
}


