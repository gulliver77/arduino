/*
13-05-2022
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

#define base 0
#define bras 1
#define avantbras 2
#define poignet 3 
#define  main 4
#define pince 5

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
#define s2  7 //fil violet
#define s3  8 //fil blanc
#define out 12 //fil vert

byte inbase, inbras, inavantbras, inpoignet, inmain, inpince ;
byte posbase, posbras, posavantbras, pospoignet, posmain, pospince ;
byte consignebase, consignebras, consigneavantbras, consignepoignet, consignemain, consignepince ;

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

/*  position      base  bras  av bras   poignet   main  pince*/ 
byte repos[]={   120,  136,  94,       114,      89,   127};
byte prendre[]={ 112,  166,  47,       40,       86,   135};// 123 prise
byte testcouleur[]={164,  171,  60,     19,      89,   125};
/*byte boiterouge[]={55, 75, 97, 110,  66, 130};
byte boitevert[]={38,  126,  30, 110,  66, 130};
byte boitebleu[]={14,  125,  28, 110,  66, 130};*/
byte boite[3][6]={{55, 75, 97, 110,  66, 130},//rouge
                {38,  126,  30, 110,  66, 130},
                {14,  125,  28, 110,  66, 130}};//rouge jaune bleu

//variable des couleurs
byte rouge = 0;
byte vert = 0;
byte bleu = 0;

// tableau min/max/init des servomoteurs
byte minbase=0,        maxbase=180,        initbase=116,
    minbras=0,       maxbras=180,        initbras=143,
    minavantbras=5,  maxavantbras=135,   initavantbras=28,
    minpoignet=3,    maxpoignet=135,     initpoignet=35,
    minmain=45,       maxmain=135,        initmain=98,
    minpince=80,     maxpince=155,       initpince=145;

boolean mode=1;

Servo servobase, servobras, servoavantbras, servopoignet, servomain, servopince;  // create servo objects to control a servo 

/***************
    SETUP
****************/
 
void setup() {
  Serial.begin(9600);
  pinMode(bp,INPUT_PULLUP);
  pinMode(s2,OUTPUT);
  pinMode(s3,OUTPUT);
  pinMode(out,INPUT);
  mode=digitalRead(bp);   //---lecture du bouton poussoir pour déterminer le mode
//---attribution des 'attach' 
  servobase.attach(3);servobase.write(initbase); delay(delaiSetup); 
  servobras.attach(5);servobras.write(initbras); delay(delaiSetup);   
  servoavantbras.attach(6); servoavantbras.write(initavantbras); delay(delaiSetup); 
  servopoignet.attach(9); servopoignet.write(initpoignet); delay(delaiSetup); 
  servomain.attach(10); servomain.write(initmain); delay(delaiSetup);
  servopince.attach(11); servopince.write(initpince);
  
  
  Serial.print(" mode : " + String(mode));
  
  delay(1000);
  if(mode) frepos();
  delay(5000);
}

/****************
  LOOP
*****************/
 
void loop() {
 if(mode) automatique(); else potards();//mode auto si False, potards si True
}


/***********************************************
 * 
 * robot en mode automatique
 * 
 **********************************************/
void automatique()
{
//Serial.println("mode auto");
  delay(1000);
  if (!digitalRead(bp)) 
  {
    fprendre();
    delay(100);
    fechap();
    delay(100);
    int i=20;
    int couleur=0;
    do 
    {
    fcouleur();
 Serial.println(" ROUGE: " + String(rouge));
 Serial.println(" VERT: " + String(vert));
 Serial.println(" BLEU: " + String(bleu));
 Serial.print("i : ");Serial.println(i);
 Serial.print("couleur : ");Serial.println(couleur);

   if((rouge > 5 && rouge < 11) && (vert > 12 && vert < 17) && (bleu > 10 && bleu < 15))
   {
   Serial.println("COULEUR ROUGE");
   couleur=1;
   }
   if((rouge > 3 && rouge < 7) && (vert > 6 && vert < 10) && (bleu > 8 && bleu < 13))
   {
   Serial.println("COULEUR JAUNE");
   couleur=2;
   }
   if((rouge > 11 && rouge < 15) && (vert > 11 && vert < 15) && (bleu > 7 && bleu < 11))
   {
   Serial.println("COULEUR BLEU");
   couleur=3;
   }
   delay(500);
   i--;
   }
   while ((i>0) & (couleur==0));
   Serial.println("sortie ");
  }
}

/****************************************************************
 * 
 * fonctions action du robot
 * 
 ***************************************************************/
 
/******* fonction repos****/
void frepos()
{
//Serial.println("repos ! ");
  fbras(repos[bras]);
  favantbras(repos[avantbras]);
  fpoignet(repos[poignet]);
}

/******* fonction echap (position intermédiaire du robot)****/
void fechap()
{
  Serial.println("echappe ! ");
  fbras(servobras.read()-20);
  favantbras(servoavantbras.read()+20);
}
/******* fonction prendre*****/
void fprendre()
{
Serial.println("prendre ! ");
  fpince(minpince);//ouverture maximale de la pince 
  fmain(prendre[main]);
  fbase(prendre[base]);
  fpoignet(prendre[poignet]);
  favantbras(prendre[avantbras]);
  fbras(prendre[bras]);
  fpince(prendre[pince]);
}

/******* fonction couleur*****/
void fcouleur()
{
Serial.println("test de la couleur ! ");
  fmain(testcouleur[main]);
  fbase(testcouleur[base]);
  fpoignet(testcouleur[poignet]); 
  favantbras(testcouleur[avantbras]); 
  fbras(testcouleur[bras]); 
  lecturecouleur();
}

/******* fonction boite rouge*****/
/* void fboiterouge()
{
Serial.println("boite rouge ! ");
  //fpince(minpince);  inutile dans cette fonction
  fmain(boiterouge[main]);
  fbase(boiterouge[base]);
  fpoignet(boiterouge[poignet]);
  favantbras(boiterouge[avantbras]);
  fbras(boiterouge[bras]);
  fpince(boiterouge[pince]);
}

void fboitevert()
{
Serial.println("boite verte ! ");
  //fpince(minpince);  inutile dans cette fonction
  fmain(boitevert[main]);
  fbase(boitevert[base]);
  fpoignet(boitevert[poignet]);
  favantbras(boitevert[avantbras]);
  fbras(boitevert[bras]);
  fpince(boitevert[pince]);
}

void fboitebleu()
{
Serial.println("prendre ! ");
  //fpince(minpince);  inutile dans cette fonction
  fmain(boitebleu[main]);
  fbase(boitebleu[base]);
  fpoignet(boitebleu[poignet]);
  favantbras(boitebleu[avantbras]);
  fbras(boitebleu[bras]);
  fpince(boitebleu[pince]);
}
*/

/****************************************************************
 * 
 * gestion des différents servomoteurs
 * 
 ***************************************************************/
 
 /***** gestion de la base****/
void fbase(byte val_finale) 
{ 
  do
  {
   posbase = servobase.read();
   if (posbase  < val_finale)servobase.write(posbase +1);else if(posbase  > val_finale) servobase.write(posbase -1); 
  delay(300);
  }
  while (posbase != val_finale); 
}


/****** gestion du bras *****/
void fbras(byte val_finale) 
{
  do
  {
   posbras = servobras.read();
   if (posbras  < val_finale)servobras.write(posbras +1);else if(posbras  > val_finale) servobras.write(posbras -1); 
  delay(250);
  }
  while (posbras != val_finale); 
}

/*******gestion de l'avantbras****/
void favantbras(byte val_finale)
{ 
   do
  {
   posavantbras = servoavantbras.read();
   if (posavantbras  < val_finale)servoavantbras.write(posavantbras +1);else if(posavantbras  > val_finale) servoavantbras.write(posavantbras -1); 
  delay(100);
  }
  while (posavantbras != val_finale);
}

/*******gestion du poignet******/
void fpoignet(byte val_finale)
{ 
  do
  {
   pospoignet = servopoignet.read();
   if (pospoignet  < val_finale)servopoignet.write(pospoignet +1);else if(pospoignet  > val_finale) servopoignet.write(pospoignet -1); 
  delay(100);
  }
  while (pospoignet != val_finale); 
}

/****** gestion de la main ****/
void fmain(byte val_finale)
{ 
  do
  {
   posmain = servomain.read();
   if (posmain  < val_finale)servomain.write(posmain +1);else if(posmain  > val_finale) servomain.write(posmain -1); 
  delay(100);
  }
  while (posmain != val_finale);   
}

/***** gestion de la pince *****/
void fpince(byte val_finale) // fonction qui gere le servomoteur de la pince
{ 
  do
  {
   pospince = servopince.read();
   if (pospince  < val_finale)servopince.write(pospince +1);else if(pospince  > val_finale) servopince.write(pospince -1); 
  delay(50);
  }
  while (pospince != val_finale);
}


/***********************************************
 * 
 * robot en mode manuel via les potentomètres
 * 
 **********************************************/
void potards() //---
{
// Serial.println("mode potards");
  
  consignebase = map(analogRead(potbase), 1023, 0, minbase, maxbase); 
  posbase = servobase.read();
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
 
/***** *************************
 affichage pour contrôle
*******************************/
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

/********************
 * lecture  couleur
 * ***************/

void lecturecouleur() {
 digitalWrite(s2, LOW);
 digitalWrite(s3, LOW);
 rouge = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);

 digitalWrite(s3, HIGH);
 bleu = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);

 digitalWrite(s2, HIGH);
 vert = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH); 
}
