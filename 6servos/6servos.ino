/*
12-05-2022
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
#define delaiSetup 20

// import AB (7-04-2022)
int inbase, inbras, inavantbras, inpoignet, inmain, inpince ;
int posbase, posbras, posavantbras, pospoignet, posmain, pospince ;
int consignebase, consignebras, consigneavantbras, consignepoignet, consignemain, consignepince ;


// tableau min/max/init des servomoteurs
int minbase=0,        maxbase=180,        initbase=82,
    minbras=0,       maxbras=135,        initbras=1,
    minavantbras=45,  maxavantbras=135,   initavantbras=85,
    minpoignet=45,    maxpoignet=135,     initpoignet=86,
    minmain=45,       maxmain=135,        initmain=88,
    minpince=100,     maxpince=155,       initpince=130;


 
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
 
//gestion base (AB 7-04-2022)
  consignebase = map(analogRead(potbase), 0, 1023, minbase, maxbase); //lecture du potentiomètre de la base et transformation en degrés entre minbase et maxbase
  posbase = servobase.read();//lecture de la position du servo de la base
  if (posbase  < consignebase)servobase.write(posbase +1);else if(posbase  > consignebase) servobase.write(posbase -1); 

//gestion bras
  consignebras = map(analogRead(potbras), 0, 1023, minbras, maxbras);
  posbras = servobras.read();
  if (posbras  < consignebras)servobras.write(posbras +1);else if(posbras  > consignebras) servobras.write(posbras -1); 

 //gestion avantbras  
  consigneavantbras = map(analogRead(potavantbras), 0, 1023, minavantbras, maxavantbras);
  posavantbras = servoavantbras.read();
  if (posavantbras  < consigneavantbras)servoavantbras.write(posavantbras +1);else if(posavantbras  > consigneavantbras) servoavantbras.write(posavantbras -1); 

 //gestion poignet  
  consignepoignet = map(analogRead(potpoignet), 0, 1023, minpoignet, maxpoignet);
  pospoignet = servopoignet.read();
  if (pospoignet  < consignepoignet)servopoignet.write(pospoignet +1);else if(pospoignet  > consignepoignet) servopoignet.write(pospoignet -1); 

//gestion main  
  consignemain = map(analogRead(potmain), 0, 1023, minmain, maxmain);
  posmain = servomain.read();
  if (posmain  < consignemain)servomain.write(posmain +1);else if(posmain  > consignemain) servomain.write(posmain -1); 

//gestion pince  
  consignepince = map(analogRead(potpince), 0, 1023, minpince, maxpince);
  pospince = servopince.read();
  if (pospince  < consignepince)servopince.write(pospince +1);else if(pospince  > consignepince) servopince.write(pospince -1); 

  affdebug();

}

void affdebug()
{
// controle affichage entrée potentiomètre, sortie valeur map 
//  Serial.println("----------");
 Serial.print(" base : " + String(posbase));
  Serial.print(" bras : " + String(posbras));
  Serial.print(" avantbras  : "  + String(posavantbras));
  Serial.print(" poignet : " + String(pospoignet));
  Serial.print(" main : " + String(posmain)); 
  Serial.println(" pince : " + String(pospince));
}
