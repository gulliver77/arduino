#include <Arduino.h>
extern String receptionCam;
//define the L298n IO pin
#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11

void InitPin ()
{
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
void Mouvement (boolean DirectionDroit, int AccelerationDroit, boolean DirectionGauche, int AccelerationGauche )
{
  static int VitesseGauche, VitesseDroit;
  Serial.print("VitesseGauche ");  Serial.print(VitesseGauche);Serial.print(" VitesseDroit ");Serial.println(VitesseDroit);

  if (DirectionDroit)
  {
    digitalWrite(IN1, HIGH);//moteur droit avance
    digitalWrite(IN2, LOW);
  }
  else
  {
    digitalWrite(IN1, LOW);//moteur droit recule
    digitalWrite(IN2, HIGH);
  }

if (DirectionGauche)
{
  digitalWrite(IN3, LOW);//moteur gauche
  digitalWrite(IN4, HIGH);  //go forward
}
else
{
  digitalWrite(IN3, HIGH);//moteur gauche
  digitalWrite(IN4, LOW);  //go forward
}
analogWrite(ENA, VitesseDroit);
digitalWrite(ENB, VitesseGauche);
}

void traitementBouton() {
  //Serial.print("receptionCam  ");
  //Serial.println(receptionCam);

  if (receptionCam.indexOf("Forward") != -1) {
    Mouvement (true, 255, true, 255 );//avant
    Serial.println(receptionCam);
  } else if (receptionCam.indexOf("Backward") != -1) {
    Mouvement (false, 110, false, 110 );//arriere
    Serial.println(receptionCam);
  } else if (receptionCam.indexOf("Stop") != -1) {
    Mouvement (true, 0, true, 0 );
    Serial.println(receptionCam);
  } else if (receptionCam.indexOf("Left") != -1) {
    Serial.print("resultat test  ");
    Serial.println(receptionCam);
  } else if (receptionCam.indexOf("Right") != -1) {
    Serial.print("resultat test  ");
    Serial.println(receptionCam);
  }
  receptionCam = "";
}