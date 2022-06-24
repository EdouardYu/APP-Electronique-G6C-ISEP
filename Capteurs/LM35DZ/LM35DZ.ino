#include <SimpleTimer.h>

#define temperatureLM35 A7

SimpleTimer timer;

int temperatureBrute;
float temperatureCelcius;

void setup() {
  Serial.begin(9600);
  timer.setInterval(1000,lectureCapteurs);
}

void loop() {
  timer.run();
}

void lectureCapteurs() {
  temperatureBrute = analogRead(temperatureLM35);
  temperatureCelcius = (temperatureBrute * 3.3 * 100) / 4095; 
  Serial.println(temperatureBrute);
}
