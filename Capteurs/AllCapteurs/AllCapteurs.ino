#include <transmissionTrames.h>
#include <MICS_VZ_89TE.h>
#include <DHT.h>
#include <GP2YDustSensor.h>
#include <SimpleTimer.h>


#define VZ89_PIN 25
#define DHT_PIN 26
#define MAX_PIN A2
#define SHARP_LED_PIN 28
#define SHARP_VO_PIN A0
#define LM35DZ_PIN A11

MICS_VZ_89TE cov;
DHT dht(DHT_PIN, DHT11);
GP2YDustSensor dustSensor(GP2YDustSensorType::GP2Y1010AU0F, SHARP_LED_PIN, SHARP_VO_PIN);
SimpleTimer timer;

float CO2_I2C;
float COV_I2C;
float CO2_PWM;
float COV_PWM;
float CO2;
float COV;
int temperatureDHT11;
int humidite;
int dustDensity;
const int sampleWindow = 5000; 
unsigned int sample;
unsigned long startMillis;
unsigned int peakToPeak;
unsigned int signalMax;
unsigned int signalMin;
double dB;  
int temperatureBruteLM35DZ;
float temperatureCelciusLM35DZ;

String trameTemperatureDHT11;
String trameTemperatureLM35DZ;
String trameHumidite;
String trameMicro;
String trameCOV;
String trameCO2;
String trameDust;
int count = 0;

void setup() {
  Serial1.begin(9600);
  Serial1.setTimeout(5000),
  cov.begin();
  cov.beginPWM(VZ89_PIN);
  dht.begin();
  dustSensor.begin();
  timer.setInterval(5000,lectureCapteurs);
}

void loop() {
  startMillis= millis(); 
  peakToPeak = 0;     
  signalMax = 0;
  signalMin = 4095;
  while (millis() - startMillis < sampleWindow) {
    sample = analogRead(MAX_PIN); 
    if (sample < 4095) { 
      if (sample > signalMax) {
        signalMax = sample;  
      }
      else if (sample < signalMin) {
        signalMin = sample; 
      }
    }
  }
  peakToPeak = signalMax - signalMin;
  dB = 20*log(peakToPeak * 25 / 4095);
  //Serial.print("Décibel du son : ");
  //Serial.print(dB);
  //Serial.println(" dB");
  trameMicro = transmissionTrames("g6c", "Lecture", "Son", 1, int(dB), count).Create_Trame();
  Serial1.println(trameMicro);
  timer.run();
}

void lectureCapteurs() {
  cov.readSensor();
  cov.readSensorPWM();
  CO2_I2C = cov.getCO2();
  COV_I2C = cov.getVOC();
  CO2_PWM = cov.getCO2PWM();
  COV_PWM = cov.getVOCPWM();
  temperatureDHT11 = dht.readTemperature();
  humidite = dht.readHumidity();
  dustDensity = dustSensor.getDustDensity(); 
  temperatureBruteLM35DZ = analogRead(LM35DZ_PIN);
  temperatureCelciusLM35DZ = (temperatureBruteLM35DZ * 3.3 * 100) / 4095;
  //Serial.print("taux de CO2 : ");
  if(CO2_I2C > 400 && CO2_PWM > 400){
    CO2 = cov.getAveragedCO2();
    //Serial.print(CO2);
    //Serial.println(" ppm");
    trameCO2 = transmissionTrames("g6c", "Lecture", "CO2", 1, int(CO2), count).Create_Trame();
  }
  else if(CO2_I2C > 400 && CO2_PWM <= 400){
    //Serial.print(CO2_I2C);
    //Serial.println(" ppm");
    trameCO2 = transmissionTrames("g6c", "Lecture", "CO2", 1, int(CO2_I2C), count).Create_Trame();
  }
  else if(CO2_I2C <= 400 && CO2_PWM > 400){
    //Serial.print(CO2_PWM);
    //Serial.println(" ppm");
    trameCO2 = transmissionTrames("g6c", "Lecture", "CO2", 1, int(CO2_PWM), count).Create_Trame();
  }
  else{
    //Serial.println("valeurs erronées");
    trameCO2 = transmissionTrames("g6c", "Lecture", "CO2", 1, 0, count).Create_Trame();
  }
  //Serial.print("taux de COV : ");
  if(COV_I2C > 0 && COV_PWM > 0){
    if(COV_PWM - COV_I2C < 50){
      COV = cov.getAveragedVOC();
      //Serial.print(COV);
      //Serial.println(" ppb");
      trameCOV = transmissionTrames("g6c", "Lecture", "COV", 1, int(COV), count).Create_Trame();
    } else{
      //Serial.print(COV_PWM);
      //Serial.println(" ppb");
      trameCOV = transmissionTrames("g6c", "Lecture", "COV", 1, int(COV_PWM), count).Create_Trame();
    }
  }
  else if(COV_I2C > 0 && COV_PWM <= 0){
    //Serial.print(COV_I2C);
    //Serial.println(" ppm");
    trameCOV = transmissionTrames("g6c", "Lecture", "COV", 1, int(COV_I2C), count).Create_Trame();
  }
  else if(COV_I2C <= 0 && COV_PWM > 0){
    //Serial.print(COV_PWM);
    //Serial.println(" ppb");
    trameCOV = transmissionTrames("g6c", "Lecture", "COV", 1, int(COV_PWM), count).Create_Trame();
  }
  else{
    //Serial.println("valeurs erronées");
    trameCOV = transmissionTrames("g6c", "Lecture", "COV", 1, 0, count).Create_Trame();
  }
  //Serial.print("Température DHT11 : ");
  //Serial.print(temperatureDHT11);
  //Serial.println(" °C");
  //Serial.print("Humidité : ");
  //Serial.print(humidite);
  //Serial.println(" % HR");
  //Serial.print("Densité de poussière: ");
  //Serial.print(dustDensity);
  //Serial.println(" µg/m^3");
  //Serial.print("Température LM35DZ : ");
  //Serial.print(temperatureCelciusLM35DZ);
  //Serial.println(" °C");
  trameTemperatureDHT11 = transmissionTrames("g6c", "Lecture", "Temperature", 1, temperatureDHT11, count).Create_Trame();
  trameTemperatureLM35DZ = transmissionTrames("g6c", "Lecture", "Temperature", 2, int(temperatureCelciusLM35DZ), count).Create_Trame();
  trameHumidite = transmissionTrames("g6c", "Lecture", "Humidite", 1, humidite, count).Create_Trame();
  trameDust = transmissionTrames("g6c", "Lecture", "Dust", 1, dustDensity, count).Create_Trame();
  if (count == 65535) {
      count = 0;
    }
  else {
      count++;
    }
  Serial1.println(trameTemperatureDHT11);
  Serial1.println(trameTemperatureLM35DZ);
  Serial1.println(trameHumidite);
  Serial1.println(trameCOV);
  Serial1.println(trameCO2);
  Serial1.println(trameDust);
}
