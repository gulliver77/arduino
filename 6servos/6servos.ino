/*  Code du robot "tri couleurs" */
/* v2023-01-31 */

```
/*
  pattes libres sur l'arduino
  serie 1 et2
  i2c A4 et A5
  13 (led)

  pb appuyé au démarrage => potentiometre sinon automatique
*/

#include <Servo.h>

//declaration PIN pour les servos
#define base 0
#define bras 1
#define avantbras 2
#define poignet 3
#define  main 4
#define pince 5

//déclaration capteur couleur
#define s2  7 //fil violet
#define s3  8 //fil blanc
#define out 12 //fil vert

//déclaration bouton poussoir
#define bp 4

/* déclaration PIN pour les 6 potards */
#define potbase A0
#define potbras A1
#define potavantbras A2
#define potpoignet A3
#define potmain A6
#define potpince A7

// déclarations diverses
#define delaiSetup 20

//variables programme
byte posbase, posbras, posavantbras, pospoignet, posmain, pospince ;
byte consignebase, consignebras, consigneavantbras, consignepoignet, consignemain, consignepince ;

/*  positions servos      base       bras   av bras   poignet    main     pince*/
byte repos[] = {           118,      130,    36,         30,      90,     155};
byte prendre[] = {        113,      150,    85,         3,       88,     135};//pince ouverte 80, 135=prise
byte testcouleur[] = {     180,      140,    62,         15,      5,      135};
byte destination[4][6] = {{142,      158,    101,         3,     103,      80}, //jeter
  { 54,       180,    108,        122,    85,       80},//rouge
  { 35,       152,    105,        67,     87,       80},//jaune
  { 13,       150,    105,        55,     87,       80}
};//bleu

//variable des couleurs fondamentales détectées par le capteur
byte rouge = 0; byte vert = 0; byte bleu = 0;

// tableau min/max/init des servomoteurs
byte minbase = 0,        maxbase = 180,        initbase = 118,
     minbras = 0,       maxbras = 180,        initbras = 130,
     minavantbras = 5,  maxavantbras = 135,   initavantbras = 36,
     minpoignet = 3,    maxpoignet = 175,     initpoignet = 30,
     minmain = 5,       maxmain = 175,        initmain = 90,
     minpince = 80,     maxpince = 155,       initpince = 155;

boolean mode = 1;// mode 1=auto, mode 0=potentiomètres

// create servo objects to control a servo
Servo servobase, servobras, servoavantbras, servopoignet, servomain, servopince;

/***************
    SETUP
****************/
void setup() {
  Serial.begin(9600);
  delay(500);
  Serial.println("Programme 6servos.ino 23-06-2022 (Gulliver)");
  pinMode(bp, INPUT_PULLUP); // en entrée tiré au +5v
  pinMode(s2, OUTPUT); // s2-s3 commutateurs de filtres capteur
  pinMode(s3, OUTPUT);
  pinMode(out, INPUT); // entrée capteur couleur
  //---lecture du bouton poussoir pour déterminer le mode
  attache_servos();
  Serial.println(" mode : " + String(mode));
  //  delay(5000);
  mode = digitalRead(bp); // mode=1 si non appuyé
  if (mode)
  {
    frepos();
    detache_servos();
  }
  //  delay(500);
}

/****************
  LOOP
*****************/
void loop() {
  if (mode) automatique(); else potards(); //mode auto si 1, potards si 0
}

/***********************************************
   robot en mode automatique
 **********************************************/
void automatique()
{
  Serial.println("Attente appui bp");
  delay(1000);
  if (!digitalRead(bp))
  {
    attache_servos();
    fprendre(); // executer la fonction fprendre qui positione le bras  à postion prendre
    delay(100);
    fechap();   // executer la fonction fechap qui eloigne le bras du plateau
    delay(100);
    fcouleur(); // executer la fonction fcouleur qui positione le bras  à postion couleur
    delay(1000);
    int nbtest = 10; int couleurValide = 0 ; int couleurPrecedente = 0;//double test (si couleur non reconnue max 10 test)
    do
    {
      lecturecouleur();
      if ((rouge > 5 && rouge < 11) && (vert > 12 && vert < 17) && (bleu > 10 && bleu < 15))
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
      else if ((rouge > 3 && rouge < 7) && (vert > 6 && vert < 10) && (bleu > 8 && bleu < 13))
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
      else if ((rouge > 11 && rouge < 15) && (vert > 11 && vert < 15) && (bleu > 7 && bleu < 11))
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
      Serial.print(".");
    }
    while ((nbtest > 0) & (couleurValide == 0));
    Serial.println("");

    fdestination(couleurValide);
    frepos();
    detache_servos();
  }
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
/****************************************************************

   fonctions action du robot

 ***************************************************************/

/******* fonction positionnement du robot en repos ****/
void frepos()
{
  fbase(repos[base]);
  fmain(repos[main]);
  fpince(repos[pince]);
  fbras(repos[bras]);
  fpoignet(repos[poignet]);
  favantbras(repos[avantbras]);
  Serial.println("Dodo robot ! ");
}

/******* fonction echap (relèvement bras avanbras du robot pour échappement plateau)****/
void fechap()
{
  fbras(servobras.read() - 20);
  favantbras(servoavantbras.read() + 20);
}

/******* fonction positionnement pour prendre l'objet*****/
void fprendre()
{
  Serial.println("prendre ! ");
  fpince(minpince);//minpince = ouverture maximale de la pince
  fmain(prendre[main]);
  fbase(prendre[base]);
  fpoignet(prendre[poignet]);
  favantbras(prendre[avantbras]);
  fbras(prendre[bras]);
  fpince(prendre[pince]);
}

/******* fonction de positionnement pour la lecture de la couleur de l'objet*****/
void fcouleur()
{
  Serial.println("test de la couleur ! ");
  fbase(testcouleur[base]);
  fpoignet(testcouleur[poignet]);
  favantbras(testcouleur[avantbras]);
  fmain(testcouleur[main]);
  fbras(testcouleur[bras]);
  fbase((testcouleur[base] - 4)); //rotation base pour coller au capteur
}

/******* fonction destination boite (0 = jeter, 1 = rouge, 2 = jaune 3 = bleu) *****/
void fdestination(int couleurboite)
{
  fechap();
  if (couleurboite != 0) Serial.println(" déposer dans boite : " + String(couleurboite));
  else Serial.println(" couleur hors couleurs définies, on dépose poubelle !");
  fmain(destination[couleurboite][main]);
  fbase(destination[couleurboite][base]);
  fpoignet(destination[couleurboite][poignet]);
  favantbras(destination[couleurboite][avantbras]);
  fbras(destination[couleurboite][bras]);
  fpince(destination[couleurboite][pince]);
  if (couleurboite == 0)
  {
    fechap();
  }
}

/****************************************************************
   gestion des différents servomoteurs
 ***************************************************************/
/***** gestion de la base****/
void fbase(byte val_finale)
{
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
  do
  {
    pospince = servopince.read();
    if (pospince  < val_finale)servopince.write(pospince + 1); else if (pospince  > val_finale) servopince.write(pospince - 1);
    delay(50);
  }
  while (pospince != val_finale);
}

/***********************************************
   robot en mode manuel via les potentiomètres
 **********************************************/
void potards()
{
  Serial.println("mode potards");
  consignebase = map(analogRead(potbase), 1023, 0, minbase, maxbase);
  posbase = servobase.read();
  if (posbase  < consignebase)servobase.write(posbase + 1); else if (posbase  > consignebase) servobase.write(posbase - 1);

  //gestion bras
  consignebras = map(analogRead(potbras), 1023, 0, minbras, maxbras);
  posbras = servobras.read();
  if (posbras  < consignebras)servobras.write(posbras + 1); else if (posbras  > consignebras) servobras.write(posbras - 1);

  //gestion avantbras
  consigneavantbras = map(analogRead(potavantbras), 0, 1023, minavantbras, maxavantbras);
  posavantbras = servoavantbras.read();
  if (posavantbras  < consigneavantbras)servoavantbras.write(posavantbras + 1); else if (posavantbras  > consigneavantbras) servoavantbras.write(posavantbras - 1);

  //gestion poignet
  consignepoignet = map(analogRead(potpoignet), 1023, 0, minpoignet, maxpoignet);
  pospoignet = servopoignet.read();
  if (pospoignet  < consignepoignet)servopoignet.write(pospoignet + 1); else if (pospoignet  > consignepoignet) servopoignet.write(pospoignet - 1);

  //gestion main
  consignemain = map(analogRead(potmain), 0, 1023, minmain, maxmain);
  posmain = servomain.read();
  if (posmain  < consignemain)servomain.write(posmain + 1); else if (posmain  > consignemain) servomain.write(posmain - 1);

  //gestion pince
  consignepince = map(analogRead(potpince), 0, 1023, minpince, maxpince);
  pospince = servopince.read();
  if (pospince  < consignepince)servopince.write(pospince + 1); else if (pospince  > consignepince) servopince.write(pospince - 1);

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
   lecture  couleur
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
```
