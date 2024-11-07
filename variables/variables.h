// variables.h
#ifndef VARIABLES_H
#define VARIABLES_H

#include <Arduino.h>

extern int compteur;       // Variable entière globale
extern float temperature;  // Variable de température globale
extern bool etatSysteme;   // Indicateur d'état global

void afficherEtat();

#endif

