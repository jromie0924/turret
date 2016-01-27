#include <Servo.h>

#define pin 4
//lower bound 60
//upper bound 120
Servo servo;
void setup() {
  // initialize serial:
  Serial.begin(9600);
  servo.attach(pin);
  servo.write(90);
  delay(1000);
  //pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

void loop() {
}


void serialEvent() {
  if (Serial.available()) {
    // get the new byte:
     int inInt = Serial.parseInt();
     servo.write(inInt);
     delay(300);
  }
}


