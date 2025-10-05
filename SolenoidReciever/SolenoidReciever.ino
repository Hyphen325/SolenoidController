#include <Arduino.h>

const int solenoidPins[] = {2, 3, 4, 5, 6, 7}; // Solenoid pins???? Idk what they'll be on yet
const int numSolenoids = 6;

void setup() {
  Serial.begin(9600); // Going to use 9600 baud rate, could use something else
  for (int i = 0; i < numSolenoids; i++) {
    pinMode(solenoidPins[i], OUTPUT); 
    digitalWrite(solenoidPins[i], LOW);
  }
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    processCommand(command);
  }
}

void processCommand(const String& command) {
  int solenoidId;
  int setpoint;
  if (sscanf(command.c_str(), "SET %d %d", &solenoidId, &setpoint) == 2) {
    if (solenoidId >= 0 && solenoidId < numSolenoids) {
      digitalWrite(solenoidPins[solenoidId], setpoint ? HIGH : LOW);
      Serial.println("Solenoid Set!!!!!!! Maybe");
    } else {
      Serial.println("Invalid solenoid ID");
    }
  } else {
    Serial.println("Invalid command format");
  }
}
