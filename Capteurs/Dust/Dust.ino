#include <GP2YDustSensor.h>

#define SHARP_LED_PIN 24  // Sharp Dust/particle sensor Led Pin
#define SHARP_VO_PIN A7    // Sharp Dust/particle analog out pin used for reading 

GP2YDustSensor dustSensor(GP2YDustSensorType::GP2Y1010AU0F, SHARP_LED_PIN, SHARP_VO_PIN);
int dustDensity;
int runningAverage;

void setup() {
  Serial.begin(9600);
  //dustSensor.setBaseline(0.4); // set no dust voltage according to your own experiments
  //dustSensor.setCalibrationFactor(1.1); // calibrate against precision instrument
  dustSensor.begin();
}

void loop() {
  dustDensity = dustSensor.getDustDensity();
  runningAverage = dustSensor.getRunningAverage();
  Serial.print("Dust density: ");
  Serial.print(dustDensity);
  Serial.println(" µg/m^3");
  Serial.print("Running average: ");
  Serial.print(runningAverage);
  Serial.println(" µg/m^3");
  delay(1000);
}
