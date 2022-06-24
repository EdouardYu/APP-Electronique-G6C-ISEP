#include <MICS_VZ_89TE.h>
#include <SimpleTimer.h>

#define COV_PIN 25

MICS_VZ_89TE cov;
SimpleTimer timer;

float CO2_I2C;
float COV_I2C;
float CO2_PWM;
float COV_PWM;
float CO2;
float COV;

void setup() {
  Serial.begin(9600);
  cov.begin();
  cov.beginPWM(COV_PIN);
  timer.setInterval(2000,lectureCapteurs);
}

void loop(){
  timer.run();
}

void lectureCapteurs(){
  cov.readSensor();
  cov.readSensorPWM();
  CO2_I2C = cov.getCO2();
  COV_I2C = cov.getVOC();  
  CO2_PWM = cov.getCO2PWM();
  COV_PWM = cov.getVOCPWM();
    Serial.print("taux de CO2 : ");
  if(CO2_I2C > 400 && CO2_PWM > 400){
    CO2 = cov.getAveragedCO2();
    Serial.print(CO2);
    Serial.println(" ppm");
  }
  else if(CO2_I2C > 400 && CO2_PWM <= 400){
    Serial.print(CO2_I2C);
    Serial.println(" ppm");
  }
  else if(CO2_I2C <= 400 && CO2_PWM > 400){
    Serial.print(CO2_PWM);
    Serial.println(" ppm");
  }
  else{
    Serial.println("valeurs erronées");
  }
  Serial.print("taux de COV : ");
  if(COV_I2C > 0 && COV_PWM > 0){
    if(COV_PWM - COV_I2C < 50){
      COV = cov.getAveragedVOC();
      Serial.print(COV);
      Serial.println(" ppb");
    } else{
      Serial.print(COV_PWM);
      Serial.println(" ppb");
    }
  }
  else if(COV_I2C > 0 && COV_PWM <= 0){
    Serial.print(COV_I2C);
    Serial.println(" ppm");
  }
  else if(COV_I2C <= 0 && COV_PWM > 0){
    Serial.print(COV_PWM);
    Serial.println(" ppb");
  }
  else{
    Serial.println("valeurs erronées");
  }
}
