// feuxTricolore.cpp

#include "feuxTricolore.h"
#include <Arduino.h>

// Constructeur : initialise les pins des LEDs et les configure en sortie
FeuxTricolore::FeuxTricolore(int pinVerte, int pinJaune, int pinRouge) {
    this->pinVerte = pinVerte;
    this->pinJaune = pinJaune;
    this->pinRouge = pinRouge;
    pinMode(pinVerte, OUTPUT);
    pinMode(pinJaune, OUTPUT);
    pinMode(pinRouge, OUTPUT);
}

// Méthode pour allumer la LED spécifiée par le paramètre pin
void FeuxTricolore::allumer(int pin) {
    digitalWrite(pin, HIGH);
}

// Méthode pour éteindre la LED spécifiée par le paramètre pin
void FeuxTricolore::eteindre(int pin) {
    digitalWrite(pin, LOW);

}

// Méthode pour éteindre toutes les LEDs
void FeuxTricolore::eteindreTout() {
    eteindre(pinVerte);
    eteindre(pinJaune);
    eteindre(pinRouge);
}

