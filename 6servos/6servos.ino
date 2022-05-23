/*
13-05-2022
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

occupation des pattes arduino
                  serie 1/2
                    i2c A4/A5
        capteur couleur 7/8/12
                 libres 13
                     BP 4
  pb appuyé au démarrage => potentiometre sinon automatique
  effet de bord tremblottement
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
#define bp 4

//define capteur couleur
#define s2  7
#define s3  8
#define out 12

// import AB (7-04-2022)
int inbase, inbras, inavantbras, inpoignet, inmain, inpince ;
int posbase, posbras, posavantbras, pospoignet, posmain, pospince ;
int consignebase, consignebras, consigneavantbras, consignepoignet, consignemain, consignepince ;

/*
position  base bras  av bras  poignet  main  pince  
init      116   143   28        35      98    145
boite1    14    125   28        110     66    130
boite2    38    126   30        110     66    130
boite3    55    75    97        110     66    130
prendre   141   117    62       29      98    130
repos     120   136   94        114     89    127

(non validé)
attrape   0     15    44        28      94
elevation       38
lecteur   87    45    78        20      94

*/

int repos[6]={120,   136,   94,        114,     89,   127};
int prendre[6]={138,  159,    26,      67,     89, 80};// 123 prise

// tableau min/max/init des servomoteurs
int minbase=0,        maxbase=180,        initbase=116,
    minbras=0,       maxbras=180,        initbras=143,
    minavantbras=5,  maxavantbras=135,   initavantbras=28,
    minpoignet=3,    maxpoignet=135,     initpoignet=35,
    minmain=45,       maxmain=135,        initmain=98,
    minpince=80,     maxpince=155,       initpince=145;

boolean mode=1;

//variable des couleurs
byte rouge = 0;
byte vert = 0;
byte bleu = 0;
 
Servo servobase, servobras, servoavantbras, servopoignet, servomain, servopince;  // create servo objects to control a servo 
 
void setup() {
  //--- afichage moniteur (provisoire)
  Serial.begin(9600);
  pinMode(bp,INPUT_PULLUP);
  //---lecture du bouton poussoir
  mode=digitalRead(bp); 
  //---attribution des 'attach' 
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
  Serial.print(" mode : " + String(mode));
  delay(1000);
  if(mode) frepos();
  delay(5000);
}
 
void loop() {
 if(mode) automatique(); else potards();//mode auto si False, potards si True
}

void affdebug()
{
//---controle affichage entrée potentiomètre, sortie valeur map  (provisoire)
//  Serial.println("----------");
  Serial.print(" base : " + String(posbase));
  Serial.print(" bras : " + String(posbras));
  Serial.print(" avantbras  : "  + String(posavantbras));
  Serial.print(" poignet : " + String(pospoignet));
  Serial.print(" main : " + String(posmain)); 
  Serial.println(" pince : " + String(pospince));
}

//--- robot en mode automatique
void automatique()
{
    Serial.println("mode auto");
  delay(1000);
  if (!digitalRead(bp)) fprendre();  
}

//---robot en mode manuel via les potentomètres
void potards()
{
  Serial.println("mode potards");

//gestion base (AB 7-04-2022)
  consignebase = map(analogRead(potbase), 1023, 0, minbase, maxbase); //lecture du potentiomètre de la base et transformation en degrés entre minbase et maxbase
  posbase = servobase.read();//lecture de la position du servo de la base
  if (posbase  < consignebase)servobase.write(posbase +1);else if(posbase  > consignebase) servobase.write(posbase -1); 

//gestion bras
  consignebras = map(analogRead(potbras), 1023, 0, minbras, maxbras);
  posbras = servobras.read();
  if (posbras  < consignebras)servobras.write(posbras +1);else if(posbras  > consignebras) servobras.write(posbras -1); 

//gestion avantbras  
  consigneavantbras = map(analogRead(potavantbras), 0, 1023, minavantbras, maxavantbras);
  posavantbras = servoavantbras.read();
  if (posavantbras  < consigneavantbras)servoavantbras.write(posavantbras +1);else if(posavantbras  > consigneavantbras) servoavantbras.write(posavantbras -1); 

//gestion poignet  
  consignepoignet = map(analogRead(potpoignet), 1023, 0, minpoignet, maxpoignet);
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


//--- fonction prise d'un objet
void fprendre()
{
/*  position  base bras  av bras  poignet  main  pince*/ 
Serial.println("prendre ! ");
  do
  {
   pospince = servopince.read();
   if (pospince  < prendre[5])servopince.write(pospince +1);else if(pospince  > prendre[5]) servopince.write(pospince -1); 
  delay(50);
  }
  while (pospince != prendre[5]);
  do
  {
   posbase = servobase.read();
   if (posbase  < prendre[0])servobase.write(posbase +1);else if(posbase  > prendre[0]) servobase.write(posbase -1); 
  delay(300);
  }
  while (posbase != prendre[0]);
 
 
  do
  {
   pospoignet = servopoignet.read();
   if (pospoignet  < prendre[3])servopoignet.write(pospoignet +1);else if(pospoignet  > prendre[3]) servopoignet.write(pospoignet -1); 
  delay(100);
  }
  while (pospoignet != prendre[3]);
   do
  {
   posavantbras = servoavantbras.read();
   if (posavantbras  < prendre[2])servoavantbras.write(posavantbras +1);else if(posavantbras  > prendre[2]) servoavantbras.write(posavantbras -1); 
  delay(100);
  }
  while (posavantbras != prendre[2]);
   do
  {
   posbras = servobras.read();
   if (posbras  < prendre[1])servobras.write(posbras +1);else if(posbras  > prendre[1]) servobras.write(posbras -1); 
  delay(250);
  }
  while (posbras != prendre[1]);
    do
  {
   pospince = servopince.read();
   if (pospince  < 123)servopince.write(pospince +1);else if(pospince  > 123) servopince.write(pospince -1); 
  delay(50);
  }
    while (pospince != 123);
}

void frepos()
{

//  repos     120   136   94        114     89    127
Serial.println("repos ! ");

  do
  {
   posbras = servobras.read();
   if (posbras  < repos[1])servobras.write(posbras +1);else if(posbras  > repos[1]) servobras.write(posbras -1); 
  delay(500);
  }
  while (posbras != repos[1]);
  do
  {
   posavantbras = servoavantbras.read();
   if (posavantbras  < repos[2])servoavantbras.write(posavantbras +1);else if(posavantbras  > repos[2]) servoavantbras.write(posavantbras -1); 
  delay(100);
  }
  while (posavantbras != repos[2]);
  do
  {
   pospoignet = servopoignet.read();
   if (pospoignet  < repos[3])servopoignet.write(pospoignet +1);else if(pospoignet  > repos[3]) servopoignet.write(pospoignet -1); 
  delay(100);
  }
  while (pospoignet != repos[3]);
  }
