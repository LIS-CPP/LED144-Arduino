#include <SoftwareSerial.h>
constexpr int rx = 5;
constexpr int tx = 6;
SoftwareSerial bt (rx, tx);
 
void setup() 	{
 	pinMode(rx,INPUT);
  pinMode(tx, OUTPUT);
 	Serial.begin(9600);
 	bt.begin(38400);
 	Serial.println("Ready to work!");
 	bt.println("AT");
}

void loop() {
  char c;
 	if (bt.available()) {
 			c = bt.read(); 
 			Serial.print(c);
  } 
 	if (Serial.available()) {
 			c = Serial.read(); 
 			bt.write(c);
  } 
}
