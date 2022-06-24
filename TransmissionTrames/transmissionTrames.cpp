#include "Energia.h"
#include "transmissionTrames.h"

transmissionTrames::transmissionTrames(String Equipe, String Requete, String TypeCapteur, int NumeroCapteur, int ValeurCapteur, int Count){
  equipe = Equipe;
  requete = Requete;
  typeCapteur = TypeCapteur;
  numeroCapteur = NumeroCapteur;
  valeurCapteur = ValeurCapteur;
  count = Count;
}

String transmissionTrames::Create_Trame(){
    TRA = TRAMECOURANTE;
  
    // On renseigne le numéro de groupe et on le convertit en hexadécimal sur 4 octets
    equipe.replace("g", "10");
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
    if(typeCapteur == "Temperature"){
      TYP = Temperature;
    }
    else if(typeCapteur == "Humidite"){
      TYP = Humidite;
    }
    else if(typeCapteur == "Son"){
      TYP = Son;
    }
    else if(typeCapteur == "COV"){
      TYP = COV;
    }
    else if(typeCapteur == "CO2"){
      TYP = CO2;
    }
    else if(typeCapteur == "Dust"){
      TYP = Dust;
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
