/*
07-04-2022
TODO :
remplacer par min et max
-------
Dénomination des articulations :
pince 
main
poignet 
avantbras
bras
base

attach PWM des servos
  servobase.attach(3);  
  servobras.attach(5);  
  servoavantbras.attach(6);  
  servopoignet.attach(9);   
  servomain.attach(10);
  servopince.attach(11);
*/

#include <Servo.h> 

/* déclaration PIN pour les 6 potards */
#define potbase A0 
#define potbras A1 
#define potavantbras A2 
#define potpoignet A3 
#define potmain A6 
#define potpince A7 
#define delaisAction 20
#define delaiSetup 200

int inbase, inbras, inavantbras, inpoignet, inmain, inpince ;
int outbase, outbras, outavantbras, outpoignet, outmain, outpince ;

// tableau min/max/init des servomoteurs
int minbase=0,        maxbase=180,        initbase=90,
    minbras=45,       maxbras=135,        initbras=90,
    minavantbras=45,  maxavantbras=135,   initavantbras=90,
    minpoignet=45,    maxpoignet=135,     initpoignet=90,
    minmain=45,       maxmain=135,        initmain=90,
    minpince=0,       maxpince=90,        initpince=45;


 
Servo servobase, servobras, servoavantbras, servopoignet, servomain, servopince;  // create servo objects to control a servo 
 
void setup() {
  Serial.begin(9600);
  servobase.attach(3); 
  servobase.write(initbase);
  delay(delaiSetup); 
  servobras.attach(5);
  servobras.write(initbras); 
  delay(delaiSetup);   
  servoavantbras.attach(6);
  servoavantbras.write(initavantbras); 
  delay(delaiSetup); 
  servopoignet.attach(9);   
  servopoignet.write(initpoignet);  
  delay(delaiSetup); 
  servomain.attach(10);
  servomain.write(initmain);
  delay(delaiSetup);
  servopince.attach(11);
  servopince.write(initpince);
  
}
 
void loop() {
  inbase=analogRead(potbase);
  outbase = map(inbase, 0, 1023, minbase, maxbase); // renvoi valeur pot en 0... 180 degrés
  inbras=analogRead(potbras);
  outbras = map(inbras, 0, 1023, minbras, maxbras); // renvoi valeur pot en 0... 180 degrés
  inavantbras=analogRead(potavantbras);
  outavantbras = map(inavantbras, 0, 1023, minavantbras, maxavantbras); // renvoi valeur pot en 0... 180 degrés
  inpoignet=analogRead(potpoignet);
  outpoignet = map(inpoignet, 0, 1023, minpoignet, maxpoignet); // renvoi valeur pot en 0... 180 degrés
  inmain=analogRead(potmain);
  outmain = map(inmain, 0, 1023, minmain, maxmain); // renvoi valeur pot en 0... 180 degrés
  inpince=analogRead(potpince);
  outpince = map(inpince, 0, 1023, minpince, maxpince); // renvoi valeur pot en 0... 180 degrés

// affichage sur moniteur série pour contrôle  
Serial.println("----------");
Serial.print(" base : ");Serial.print(inbase);
Serial.print(" bras : ");Serial.print(inbras);
Serial.print(" avantbras  : ");Serial.print(inavantbras);
Serial.print(" poignet : ");Serial.print(inpoignet);
Serial.print(" main : ");Serial.print(inmain);
Serial.print(" pince : ");Serial.println(inpince);
Serial.print(" base : ");Serial.print(outbase);
Serial.print(" bras : ");Serial.print(outbras);
Serial.print(" avantbras : ");Serial.print(outavantbras);
Serial.print(" poignet : ");Serial.print(outpoignet);
Serial.print(" main : ");Serial.print(outmain);
Serial.print(" pince : ");Serial.println(outpince);

 

  Servobaseaction(outbase);
//  servobras.write(outbras); 
//  servoavantbras.write(outavantbras); 
//  servopoignet.write(outpoignet);  
//  servomain.write(outmain);
//  servopince.write(outpince);
delay(1000);
}

// TODO régler le problème d'un retour manuel du potentiomètre
void Servobaseaction(int consigne)
{
  int position=servobase.read();
  if (position!=consigne)
  {
    if (position<consigne)
    {
      do
      {
        position++;
        delay(delaisAction);
        servobase.write(position);
      }
      while (position <= consigne);
    }
    else
    {
      do
      {
        position--;
        delay(delaisAction);
        servobase.write(position);
      }
      while (position >= consigne);
    }
  }
}
