#include "variables.h"

int compteur = 0;         // Définition de la variable compteur
float temperature = 0.0;  // Définition de la variable temperature
bool etatSysteme = true;  // Définition de la variable etatSysteme

void setup() {
  Serial.begin(9600);
    // Code d'initialisation
}

void loop() {
  etatSysteme=true;
  afficherEtat();
  delay(3000);
  etatSysteme=false;
  afficherEtat();
  delay(3000);
    // Code principal
}

