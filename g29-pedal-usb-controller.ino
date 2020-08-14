
#include <g29.h>


G29Pedals g29;
uint16_t a = 0;

int aPin = A7;
int bPin = A8;

uint16_t aVal = 0;
uint16_t bVal = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  g29.begin();

  aVal = analogRead(aPin);
  bVal = analogRead(bPin);

  Serial.print("a=");
  Serial.print(aVal);
  Serial.print(" b=");
  Serial.print(bVal);
  Serial.println();

  g29.setAccelerator(aVal);
  g29.setBrake(bVal);
  
  g29.end();
  delay(500);

}
