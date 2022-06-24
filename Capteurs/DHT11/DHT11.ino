//Ce code est destiné à faire passer des trames
#include <SimpleTimer.h>
#include <DHT.h>

#define DHT_PIN 23

SimpleTimer timer;
DHT dht(DHT_PIN, DHT11);

int temperature;
int humidite;

long count = 0;
String trame;
String sum;
int sumASCII;

String TRAMECOURANTE = "1"; //Trame courante
String ECRITURE = "1";
String LECTURE = "2";
String LECRITURE = "3";
String LM35DZ = "3";
String dht11 = "4";
String MAX4466 = "e";

String TRA;
String OBJ;
String REQ;
String TYP;
String NUM;
String VAL;
String TIM;
String CHK;

void setup()
{
  Serial.begin(9600);
  //Serial1.begin(9600);
  timer.setInterval(1000,lectureCapteurs);
  dht.begin();
}

void loop(){
  timer.run();
}

void lectureCapteurs() {
  humidite = dht.readHumidity();
  trame = Create_Trame("g6c", "Lecture", "DHT11", 1, humidite);
  Serial.println("Humidité : " + String(humidite) + " % HR");
  Serial.println(trame);
  //Serial1.println(trame);
}

String Create_Trame(String equipe, String requete, String typeCapteur, int numeroCapteur, int valeurCapteur){
    TRA = TRAMECOURANTE;
  
    // On renseigne le numéro de groupe et on le convertit en hexadécimal sur 4 octets
    equipe.replace("g", "1f");
    OBJ = equipe;
    if (OBJ.length() < 4){
        for (int i = 0; i <= (4 - OBJ.length()) + 1; i++) {
            OBJ = "0" + OBJ;
        }
    }

    // On décide du type de requête
    if(requete == "Lecture"){
      REQ = LECTURE;
    }
    else if(requete == "Ecriture"){
      REQ = ECRITURE;
    }
    else if(requete == "L&R"){
      REQ = LECRITURE;
    }
    else{
      REQ = LECRITURE;
    }
   
    // On décide du type de capteur
    if(typeCapteur == "LM35DZ"){
      TYP = LM35DZ;
    }
    else if(typeCapteur == "DHT11"){
      TYP = dht11;
    }
    else if(typeCapteur == "MAX4466"){
      TYP = MAX4466;
    }
    else{
      TYP = "0";
    }

    // On convertit le numéro du capteur en hexadécimal sur 2 octets
    NUM = String(numeroCapteur, HEX);
    if (NUM.length() < 2){       
        NUM = "0" + NUM;
    }

    // On convertit la valeur du capteur en hexadécimal sur 4 octets
    VAL = String(valeurCapteur, HEX);
    if (VAL.length() < 4){
        for (int i = 0; i <= (4 - VAL.length() + 1); i++) {
            VAL = "0" + VAL;
        }
    }

    // Le TimeStamp 
    TIM = String(count, HEX);
    if (TIM.length() < 4){
        for (int i = 0; i <= (4 - TIM.length()) + 1; i++) {
            TIM = "0" + TIM;
        }
    }
    if (TIM.equals("ffff")) {
      count = 0;
    }
    else {
      count++;
    }
    // On calcul le checksum des données sur 2 octets
    // On convertit chaque caractère en code ASCII décimal
    sum = TRA + OBJ + REQ + TYP + NUM + VAL + TIM;
    int len = sum.length();
    int buff[len];   
    for(int i = 0; i < len; i++){ 
      if(String(sum[i]) == "a"){
        buff[i] = 97;
      }
      else if(String(sum[i]) == "b"){
        buff[i] = 98;
      } 
      else if(String(sum[i]) == "c"){
        buff[i] = 99;
      }
      else if(String(sum[i]) == "d"){
        buff[i] = 100;
      }
      else if(String(sum[i]) =="e"){
        buff[i] = 101;
      }
      else if(String(sum[i]) == "f"){
        buff[i] = 102;
      }
      else{
        buff[i] = 48 + String(sum[i]).toInt();
      }
    }

    // On fait la somme des valeurs ASCII de toutes les caractères
    sumASCII = 0;
    for(int i = 0; i < len; i++){ 
      sumASCII += buff[i];
    }
    
    // On prend les que les valeurs des 2 premiers octets soit les valeurs inférieures à 16^2 (256)
    sumASCII %= 256;
    CHK = String(sumASCII, HEX);
    if (CHK.length() < 2){       
        CHK = "0" + CHK;
    }

    // On crée la trame en concaténant toutes les données
    trame = TRA + OBJ + REQ + TYP + NUM + VAL + TIM + CHK;

    return trame;
}
