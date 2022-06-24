#define PIN_MICRO A7

const int sampleWindow = 1000;
unsigned int sample;
unsigned long startMillis;
unsigned int peakToPeak;
unsigned int signalMax;
unsigned int signalMin;
double volts;

void setup() {
   Serial.begin(9600);  
}

void loop() {
   startMillis= millis(); 
   peakToPeak = 0;     
   signalMax = 0;
   signalMin = 4095;
   while (millis() - startMillis < sampleWindow) {
      sample = analogRead(PIN_MICRO);
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
   volts = (peakToPeak * 3.3) / 4095;
   Serial.print("Tension du son : ");
   Serial.print(volts);
   Serial.println(" V");
}
