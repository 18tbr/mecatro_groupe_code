// Code used by the P18 for the project (i.e. year 2019-2020)


#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS1 = Adafruit_MotorShield(0x60);
Adafruit_MotorShield AFMS2 = Adafruit_MotorShield(0x61);
Adafruit_MotorShield AFMS3 = Adafruit_MotorShield(0x62);
Adafruit_MotorShield AFMS4 = Adafruit_MotorShield(0x63);

// to motor port #1
Adafruit_StepperMotor *Motor100 = AFMS1.getStepper(1024, 1);
// to motor port #2
Adafruit_StepperMotor *Motor001 = AFMS1.getStepper(1024, 2);
// to motor port #1
Adafruit_StepperMotor *Motor111 = AFMS2.getStepper(1024, 1);
// to motor port #2
Adafruit_StepperMotor *Motor011 = AFMS2.getStepper(1024, 2);
// to motor port #1
Adafruit_StepperMotor *Motor110 = AFMS3.getStepper(1024, 1);
// to motor port #2
Adafruit_StepperMotor *Motor000 = AFMS3.getStepper(1024, 2);
// to motor port #1
Adafruit_StepperMotor *Motor101 = AFMS4.getStepper(1024, 1);
// to motor port #2
Adafruit_StepperMotor *Motor010 = AFMS4.getStepper(1024, 2);
  int compt = 1;

void setup() {
                 // set up Serial library at 9600 bps  , processing liaison série
  AFMS1.begin(1000);  // create with the default frequency 1.6KHz
  AFMS2.begin(1000);
  AFMS3.begin(1000);
  AFMS4.begin(1000);
  Serial.begin(9600);
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
}

void loop() {

  //Vers le haut
  float LA1[] = {0,20,0,0,20,20,0,20};
  float LB1[] = {20,0,20,20,0,0,20,0};
  //Vers le bas
  float LA2[] = {20,0,20,20,0,0,20,0};
  float LB2[] = {0,20,0,0,20,20,0,20};
  //Vers la gauche
  float LA3[] = {30,25,0,30,0,25,0,0};
  float LB3[] = {0,0,20,0,20,0,20,20};
  //Vers la droite
  float LA4[] = {0,0,30,0,20,0,30,20};
  float LB4[] = {20,20,0,20,0,20,0,0};
  Adafruit_StepperMotor *moteur[] = {Motor000, Motor001, Motor100, Motor010, Motor101, Motor011, Motor110, Motor111};
  int i = 0;
  int j = 0;
  float dif[] = {0,0,0,0,0,0,0,0};
  int pas[] = {0,0,0,0,0,0,0,0};
  int quotient[] = {0,0,0,0,0,0,0,0};
  int reste[] = {0,0,0,0,0,0,0,0};
  int restepas[] = {0,0,0,0,0,0,0,0};
  int paseffectif[] = {0,0,0,0,0,0,0,0};
  int maxpas = abs(pas[0]);



  Serial.print(" compteur =");
  Serial.println(compt);

      //CALCUL DU NOMBRE DE PAS A FAIRE
       if(compt== 1 || compt== 4){
        for (i=0; i<8; i++) {
           dif[i] = LA1[i]-LB1[i];
           pas[i] = int(dif[i]/0.09204);
        }
        Serial.println("vers le haut");
        }else if(compt==2 || compt==3){
         for (i=0; i<8; i++) {
            dif[i] = LA2[i]-LB2[i];
            pas[i] = int(dif[i]/0.09204);
        }
        Serial.println("vers le bas");
       }else if(compt==5){
        for (i=0; i<8; i++) {
           dif[i] = LA3[i]-LB3[i];
           pas[i] = int(dif[i]/0.09204);
        }
        Serial.println("vers la gauche");
       }else if(compt== 6 || compt== 7){
        for (i=0; i<8; i++) {
            dif[i] = LA4[i]-LB4[i];
            pas[i] = int(dif[i]/0.09204);
        }
        Serial.println("vers la droite");
       }else if(compt==8){
        for (i=0; i<8; i++) {
           dif[i] = LA3[i]-LB3[i];
           pas[i] = int(dif[i]/0.09204);
        }
        Serial.println("vers la gauche");
       }

       compt++;
       if(compt==9){
        compt =1;
       }

    //CALCUL NOMBRE DE PAS MAX
      for (i = 1; i<8; i++) {
       if (maxpas < abs(pas[i])) {
        maxpas=abs(pas[i]);
       }
      }
      Serial.println(maxpas);
      delay(1000);

    // CALCUL DES RESTES ET QUOTIENTS
      for (i=0; i<8; i++) {
        if( (maxpas%abs(pas[i])) > abs(pas[i])/2 ) {
          quotient[i]=(maxpas/abs(pas[i]))+1;
          reste[i]=maxpas-(abs(pas[i]) * quotient[i]);
        } else {
          quotient[i]=(maxpas/abs(pas[i]));
          reste[i]=maxpas-(abs(pas[i]) * quotient[i]);
        }

        if ( (((abs(pas[i])*quotient[i]) - maxpas) % quotient[i]) > 0) {
          restepas[i]=  ( ((abs(pas[i])*quotient[i]) - maxpas) / quotient[i] ) + 1;
        } else if( (((abs(pas[i])*quotient[i]) - maxpas) % quotient[i]) < 0)  {
          restepas[i]=  ( ((abs(pas[i])*quotient[i]) - maxpas) / quotient[i] ) - 1;
        } else {
          restepas[i]=  ( ((abs(pas[i])*quotient[i]) - maxpas) / quotient[i] );
        }
      }

    // GESTION PAS
      for (j=0; j<maxpas; j++) {
        //QUOTIENT : FAIRE UN PAS TOUT LES X QUOTIENTS
        for(i =0; i < 8; i++) {
          if( j%quotient[i] == 0 )
            if(pas[i] > 0) {
              moteur[i]->step(1, FORWARD, SINGLE);
              paseffectif[i]++;
            } else {
              moteur[i]->step(1, BACKWARD, SINGLE);
              paseffectif[i]++;
            }
          // RESTEPAS A FAIRE CORRECTION DE X PAS TOUT LES X BOUCLES
          if (restepas[i] != 0) {
            if( ( ( (maxpas-1)/restepas[i] ) % j ) == 0) {
              if(restepas[i] > 0) {
                moteur[i]->step(1, FORWARD, SINGLE);
                paseffectif[i]++;
              } else {
                moteur[i]->step(1, BACKWARD, SINGLE);
                paseffectif[i]--;
              }
            }
          }

        }

      }

    // AFFICHAGE DES TABLEAUX RESTE ET QUOTIEN POUR VERIFICATION
      Serial.print("Pas : ");
      for (i=0; i<8; i++) {
        Serial.print(pas[i]);
        Serial.print(", ");
      }
      Serial.println();
      Serial.print("Quotient : ");
      for (i=0; i<8; i++) {
        Serial.print(quotient[i]);
        Serial.print(", ");
      }
      Serial.println();
      Serial.print("Reste pas : ");
      for (i=0; i<8; i++) {
        Serial.print(restepas[i]);
        Serial.print(", ");
      }
      Serial.println();
      Serial.print("Pas effectif : ");
      for (i=0; i<8; i++) {
        Serial.print(paseffectif[i]);
        Serial.print(", ");
      }
      Serial.println();
      delay(1000);

 }
