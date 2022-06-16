/*
02-06-2022
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
//declaration PIN pour les servos
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
// base : 118 bras : 130 avantbras  : 36 poignet : 28 main : 90 pince : 155

/*  positions servos      base  bras  av bras   poignet   main  pince*/ 
byte repos[]= { 118,      130,    36,     30,       90,     155};
byte prendre[] = {113,  150,  85, 3, 88, 135};//pince ouverte 80, pince prise 130
byte testcouleur[]= {180,  140,  62,     15,      5,   135};
byte destination[4][6]= {{142, 158, 101, 3,  103, 80},//jeter
                        {54, 180, 108, 122,  85, 80},//rouge
                        {35,  152,  105, 67,  87, 80},//jaune
                        {13,  150,  105, 55,  87, 80}};//rbleu

//variable des couleurs
byte rouge = 0; byte vert = 0; byte bleu = 0;

// tableau min/max/init des servomoteurs

byte minbase=0,        maxbase=180,        initbase=118,
    minbras=0,       maxbras=180,        initbras=130,
    minavantbras=5,  maxavantbras=135,   initavantbras=36,
    minpoignet=3,    maxpoignet=175,     initpoignet=30,
    minmain=5,       maxmain=175,        initmain=90,
    minpince=80,     maxpince=155,       initpince=155;

// mode 0=auto, mode 1=potentiomètres
boolean mode=1;

// create servo objects to control a servo
Servo servobase, servobras, servoavantbras, servopoignet, servomain, servopince;   

/***************
    SETUP
****************/
 void setup() {
  Serial.begin(9600);
  pinMode(bp,INPUT_PULLUP);
  pinMode(s2,OUTPUT);
  pinMode(s3,OUTPUT);
  pinMode(out,INPUT);
//---lecture du bouton poussoir pour déterminer le mode  
  mode=digitalRead(bp);   
  attache_servos();
  Serial.print(" mode : " + String(mode)); //provisoire
  delay(5000);
  if (mode) frepos();
  delay(2000);
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
  Serial.println("Attente mise en service");
  delay(1000);
  if (!digitalRead(bp)) 
  {
    attache_servos();
    fprendre(); // executer la fonction fprendre qui positione le bras  à postion prendre
    delay(100);
    fechap();   // executer la fonction fechape qui eloigne le bras du plateau
    delay(100);
    fcouleur(); // executer la fonction fcouleur qui positione le bras  à postion couleur   
    delay(1000);    
//    int nbtest = 10; int couleur=0;
    int nbtest = 10; int couleurValide = 0 ; int couleurPrecedente = 0; 
     do 
    {
      lecturecouleur();
//    Serial.println(" ROUGE: " + String(rouge)); Serial.println(" VERT: " + String(vert)); Serial.println(" BLEU: " + String(bleu));
      if((rouge > 5 && rouge < 11) && (vert > 12 && vert < 17) && (bleu > 10 && bleu < 15))
      {
        if (couleurPrecedente == 1) 
        {
          couleurValide = 1;
          Serial.println("COULEUR ROUGE");
        }
        else
        {
          couleurPrecedente = 1;  
          Serial.println("couleur rouge à confirmer");        
        }
      }
      else if((rouge > 3 && rouge < 7) && (vert > 6 && vert < 10) && (bleu > 8 && bleu < 13))
      {
        if (couleurPrecedente == 2) 
        {
          couleurValide = 2;
          Serial.println("COULEUR JAUNE");
        }
        else
        {
          couleurPrecedente = 2; 
          Serial.println("couleur jaune à confirmer");      
        }
      }
      else if((rouge > 11 && rouge < 15) && (vert > 11 && vert < 15) && (bleu > 7 && bleu < 11))
      {
        if (couleurPrecedente == 3) 
        {
          couleurValide = 3;
          Serial.println("COULEUR BLEU");
        }
        else
        {
          couleurPrecedente = 3;  
         Serial.println("couleur bleue à confirmer");               
        }
      }
    delay(1000);
    nbtest--;
   }
   while ((nbtest > 0) & (couleurValide == 0));
   /*---------------*/  
  Serial.println(" sortie après " + String((10-nbtest)) + " tests");
   fdestination(couleurValide);
   frepos();
  }
}

/****************
 * attache et detache
 * **************/
 void attache_servos()
 {
 //---attribution des 'attach' 
  servobase.attach(3);servobase.write(initbase); delay(delaiSetup); 
  servobras.attach(5);servobras.write(initbras); delay(delaiSetup);   
  servoavantbras.attach(6); servoavantbras.write(initavantbras); delay(delaiSetup); 
  servopoignet.attach(9); servopoignet.write(initpoignet); delay(delaiSetup); 
  servomain.attach(10); servomain.write(initmain); delay(delaiSetup);
  servopince.attach(11); servopince.write(initpince); 
 
 }

void detache_servos()
{
  servobase.detach();
  servobras.detach();  
  servoavantbras.detach(); 
  servopoignet.detach();
  servomain.detach(); 
  servopince.detach();
}
/****************************************************************
 * 
 * fonctions action du robot
 * 
 ***************************************************************/
 
/******* fonction repos****/
void frepos()
{
  fbase(repos[base]);
  fmain(repos[main]);
  fpince(repos[pince]);
  fbras(repos[bras]);
  fpoignet(repos[poignet]);
  favantbras(repos[avantbras]);
  detache_servos();
  Serial.println("Dodo robot ! ");
}

/******* fonction echap (position intermédiaire du robot)****/
void fechap()
{
 // Serial.println(" ! ");
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

/******* fonction de positionnement pour la lecture de la couleur*****/
void fcouleur()
{
Serial.println("test de la couleur ! ");
  fbase(testcouleur[base]); 
  fpoignet(testcouleur[poignet]); 
  favantbras(testcouleur[avantbras]);
  fmain(testcouleur[main]);  
  fbras(testcouleur[bras]);
  fbase((testcouleur[base]-4));
}

/******* fonction destination boite (0 = jeter, 1 = rouge, 2 = jaune 3 = bleu) *****/
void fdestination(int couleurboite) 
{
  fechap();
  Serial.println(" déposer dans boite : " + String(couleurboite));
  fmain(destination[couleurboite][main]);
  fbase(destination[couleurboite][base]);
  fpoignet(destination[couleurboite][poignet]);
  favantbras(destination[couleurboite][avantbras]);
  fbras(destination[couleurboite][bras]);
  fpince(destination[couleurboite][pince]);
  if (couleurboite==0)
    { 
      Serial.println(" couleur inconnue, je laisse aux chefs ! ");      
      fechap();
    }
}

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
  delay(50);
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
  delay(50);
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
  delay(50);
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
  delay(50);
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
  delay(50);
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
 * robot en mode manuel via les potentiomètres
 * 
 **********************************************/
void potards() //---
{
 Serial.println("mode potards"); 
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
 affichage pour contrôle (provisoire)
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
void lecturecouleur()
{
 digitalWrite(s2, LOW);
 digitalWrite(s3, LOW);
 rouge = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
 digitalWrite(s3, HIGH);
 bleu = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
 digitalWrite(s2, HIGH);
 vert = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH); 
}
