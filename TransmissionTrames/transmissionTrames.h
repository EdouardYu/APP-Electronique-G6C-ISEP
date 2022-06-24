#ifndef TRANSMISSION_TRAMES_H
#define TRANSMISSION_TRAMES_H

#include "Energia.h"

class transmissionTrames {
    private:
        String TRA;
        String OBJ;
        String REQ;
        String TYP;
        String NUM;
        String VAL;
        String TIM;
        String CHK;

        String equipe;
        String requete;
        String typeCapteur;
        int numeroCapteur;
        int valeurCapteur;
        int count;

        String sum;
        long sumASCII;
        String trame;

    public:
        transmissionTrames(String Equipe, String Requete, String TypeCapteur, int NumeroCapteur, int ValeurCapteur, int Count);
        String TRAMECOURANTE = "1"; //Trame courante
        String ECRITURE = "1";
        String LECTURE = "2";
        String LECRITURE = "3";
        String Temperature = "3";
        String Humidite = "4";
        String Son = "a";
        String COV = "1";
        String CO2 = "2";
        String Dust = "5";

        
        String Create_Trame();
};

#endif
