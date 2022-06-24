// #define PIN_LED_IR A7
#define PIN_PHOTOTRANSISTOR A6

void setup()
{
    Serial.begin(9600);
    pinMode(PIN_PHOTOTRANSISTOR,INPUT);
//  pinMode(PIN_LED_IR,OUTPUT);
//  digitalWrite(PIN_LED_IR, HIGH);
}

void loop() 
{
      Serial.println(analogRead(PIN_PHOTOTRANSISTOR));
      delay(10);
}


int test ()
{
  int mesure = 0;
  
  for(int i=0;i<10;i++)
  {
    mesure = mesure + analogRead(A0);
  }
  mesure = (mesure)/5;

  return mesure;
}

void rythmeCardiaque ()
{
  int mesure;
  int mesure2;
  int periode;
  float rythme;


  mesure = analogRead(PIN_PHOTOTRANSISTOR);
  delay (10);
  mesure2 = analogRead(PIN_PHOTOTRANSISTOR);
  if (mesure2 > 1.5 * mesure)
  {
    int a = millis();
    boolean b = true;

    while (b)
    {
      mesure = analogRead(PIN_PHOTOTRANSISTOR);
      delay (10);
      mesure2 = analogRead(PIN_PHOTOTRANSISTOR);

      if (mesure2 > 1.5 * mesure)
      {
        periode = millis() - a;
        b = false;
      }
    }
    
  }

  rythme = (float) 60 / periode;
  Serial.print("Rythme cardiaque : ");
  Serial.print(periode);
  Serial.println(" bpm");   
}
