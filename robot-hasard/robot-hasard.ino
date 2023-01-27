/* utilisation de la commance memcpy
 *  pour copier un tableau dans un autre
 *  (à voir si c'est bien !
 */

/* travail du 15-12-2022 */
//---------------------------//

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

/*--- 2022-10-25 ----------------*/

int inbase, inbras, inavantbras, inpoignet, inmain, inpince ;
int posbase, posbras, posavantbras, pospoignet, posmain, pospince ;
int consignebase, consignebras, consigneavantbras, consignepoignet, consignemain, consignepince ;


/*=========================
   VARIABLES SERVOS
  ========================= */
//declaration PIN pour les servos
#define base 0
#define bras 1
#define avantbras 2
#define poignet 3
#define  main 4
#define pince 5

// tableau min/max/init des servomoteurs
/* byte minbase = 0,        maxbase = 180,        initbase = 110,
     minbras = 0,       maxbras = 180,        initbras = 130,
     minavantbras = 5,  maxavantbras = 135,   initavantbras = 36,
     minpoignet = 3,    maxpoignet = 175,     initpoignet = 30,
     minmain = 5,       maxmain = 175,        initmain = 90,
     minpince = 80,     maxpince = 155,       initpince = 155;*/

byte minbase=0,        maxbase=180,        initbase=110,//118
    minbras=0,       maxbras=180,        initbras=74,//130,
    minavantbras=5,  maxavantbras=135,   initavantbras=5,//36
    minpoignet=3,    maxpoignet=175,     initpoignet=50,//30
    minmain=0,       maxmain=180,        initmain=90,
    minpince=70,     maxpince=155,       initpince=117;//155     

// create servo objects to control a servo
Servo servobase, servobras, servoavantbras, servopoignet, servomain, servopince;

// déclarations diverses
#define delaiSetup 20

//Parametre bouton poussoir bp
const int bpPin  = 4;
int bpVal  = 0;

// mode 0=false (repos) 1=true (jeu)
boolean mode = false;

// tirageRandom pour tirage aléatoire de 1 à 10
byte tirageRandom;

// position des 6 servos  pour prendre et repos
// ordre base, bras, avantbras, poignet, main , pince
byte positionPrendre[] = {113,73,18,57,85,80};
byte prendre[] = {113,73,18,57,85,132}; //from 6bras

byte positionRepos[] = { 110, 70, 5, 50, 90, 117};
//byte repos[]= {110,70,5,50,90,117}; //from 6bras

// position des 6 servos  pour la case d'arrivée
// ordre base, bras, avantbras, poignet, main , pince
byte positionArrivee[] = {106, 73, 5, 55, 135, 155};

// définition des 6 positions servos pour les 9 cases
// ordre des servos base, bras, avantbras, poignet, main , pince
byte positionCase[9][6] = {
  { 75,80,38,57,100,155}, //case une
  { 62,98,65,57,100,155},//case 2
  { 55,116,96,57,100,155}, //case 3
  { 51,62,9,57,100,155}, //case 4
  { 43,83,41,57,100,155}, //case 5
  { 38,105,74,57,100,155},//case 6
  { 17,58,5,57,100,155}, //case 7
  { 17,80,34,57,100,155}, //case 8
  { 17,100,65,57,100,155} //case 9
};

void setup() {
  Serial.begin(9600);
  pinMode(bpPin, INPUT_PULLUP);
  delay(100);
  Serial.println(" ");
  Serial.println(" SETUP");  
  attache_servos();
  frepos(); //robot mis au repos dans le setup
  detache_servos();
}

void loop() {
  attente_bp();
  if (mode) {
    attache_servos();
    fechap();
    fprendre();    
    Serial.println("-----DÉBUT DU JEU-----");
    jeu();
    fdestination();
    Serial.println("-----FIN DU JEU-----"); 
    fechap();    
    frepos();   
    delay(2000);
  }
}


//================
//fonctions d'agrément visuel inutile dans le programme définitif
void curseur() {
  for (int i = 10; i >= 0; i--) {
    Serial.print(i); Serial.print("-");
    delay(300);
  }
  Serial.println("-");
}

/****************
   attache des servomoteurs (librairie servo)
 * **************/
void attache_servos()
{
  //---attribution des 'attach'
  servobase.attach(3); servobase.write(initbase); delay(delaiSetup);
  servobras.attach(5); servobras.write(initbras); delay(delaiSetup);
  servoavantbras.attach(6); servoavantbras.write(initavantbras); delay(delaiSetup);
  servopoignet.attach(9); servopoignet.write(initpoignet); delay(delaiSetup);
  servomain.attach(10); servomain.write(initmain); delay(delaiSetup);
  servopince.attach(11); servopince.write(initpince);
}

/****************
   détache des servomoteurs (librairie servo)
 * **************/
void detache_servos()
{
  servobase.detach();
  servobras.detach();
  servoavantbras.detach();
  servopoignet.detach();
  servomain.detach();
  servopince.detach();
}

// fonction jeu
void jeu() {
  randomSeed(millis());
  tirageRandom = random(0, 9);
  Serial.print("Tirage de la case "); Serial.println(tirageRandom + 1);
  memcpy(positionArrivee, positionCase[tirageRandom], 6); 
}

/******* fonction positionnement pour prendre l'objet*****/
void fprendre()
{
  Serial.println(" ..........");
  Serial.println("fonction prendre() ");
  memcpy(positionArrivee, positionPrendre, 6); 
  fdestination(); 
}

/* ****** fonction destination finale **** */
void fdestination()
{ 
  //fechap();
  //Serial.println(" ..........");
  //Serial.println("fonction destination()");  
  //delay(2000);
  fbase(positionArrivee[base]);
  delay(500);   
  fbras(positionArrivee[bras]);
  delay(500);
  favantbras(positionArrivee[avantbras]);
  delay(500); 
  fpoignet(positionArrivee[poignet]);
  delay(500);   
  fmain(positionArrivee[main]);
  delay(500);
  fpince(positionArrivee[pince]);
  delay(500);
}

/******* fonction positionnement du robot en repos ****/
void frepos()
{ //fechap();
  Serial.println(" ..........");
  memcpy(positionArrivee, positionRepos, 6);
  Serial.println("fonction frepos() ");
  fdestination(); 
}

/******* fonction echap (relèvement bras avanbras du robot pour échappement plateau)****/
void fechap()
{
  Serial.println(" ..........");   
  Serial.println("fonction fechap()");
    fbras(servobras.read() - 20);
    favantbras(servoavantbras.read() + 20);   
}

void fechap()
{
 // Serial.println(" ! ");
  fbras(servobras.read()-20);
  favantbras(servoavantbras.read()+20);
}
/****************************************************************
   gestion des différents servomoteurs
 ***************************************************************/
/***** gestion de la base****/
void fbase(byte val_finale)
{
  Serial.print(" rotation base : "); Serial.println(positionArrivee[base]);
    do
    {
      posbase = servobase.read();
      if (posbase  < val_finale)servobase.write(posbase + 1); else if (posbase  > val_finale) servobase.write(posbase - 1);
      delay(50);
    }
    while (posbase != val_finale);
}

/****** gestion du bras *****/
void fbras(byte val_finale)
{
  Serial.print(" rotation bras : "); Serial.println(positionArrivee[bras]);
    do
    {
      posbras = servobras.read();
      if (posbras  < val_finale)servobras.write(posbras + 1); else if (posbras  > val_finale) servobras.write(posbras - 1);
      delay(50);
    }
    while (posbras != val_finale);
}

/*******gestion de l'avantbras****/
void favantbras(byte val_finale)
{
  Serial.print(" rotation avant-bras : "); Serial.println(positionArrivee[avantbras]);
   do
    {
      posavantbras = servoavantbras.read();
      if (posavantbras  < val_finale)servoavantbras.write(posavantbras + 1); else if (posavantbras  > val_finale) servoavantbras.write(posavantbras - 1);
      delay(50);
    }
    while (posavantbras != val_finale);
}

/*******gestion du poignet******/
void fpoignet(byte val_finale)
{
  Serial.print(" rotation poignet : "); Serial.println(positionArrivee[poignet]);
   do
    {
      pospoignet = servopoignet.read();
      if (pospoignet  < val_finale)servopoignet.write(pospoignet + 1); else if (pospoignet  > val_finale) servopoignet.write(pospoignet - 1);
      delay(50);
    }
    while (pospoignet != val_finale);
}

/****** gestion de la main ****/
void fmain(byte val_finale)
{
  Serial.print(" rotation main : "); Serial.println(positionArrivee[main]);
   do
    {
      posmain = servomain.read();
      if (posmain  < val_finale)servomain.write(posmain + 1); else if (posmain  > val_finale) servomain.write(posmain - 1);
      delay(50);
    }
    while (posmain != val_finale);
}

/***** gestion de la pince *****/
void fpince(byte val_finale) // fonction qui gere le servomoteur de la pince
{
  Serial.print(" rotation pince : "); Serial.println(positionArrivee[pince]);
    do
    {
      pospince = servopince.read();
      if (pospince  < val_finale)servopince.write(pospince + 1); else if (pospince  > val_finale) servopince.write(pospince - 1);
      delay(50);
    }
    while (pospince != val_finale);
}

// attente bouton poussoir, lancé par loop()
void attente_bp() {
  mode = false;
  Serial.println("Attente bouton");
  mode = !digitalRead(bpPin);
}
