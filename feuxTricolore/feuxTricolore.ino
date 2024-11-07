#include "feuxTricolore.h"

#include "variables.h"

FeuxTricolore feux(pinVerte, pinJaune, pinRouge);

void setup() {
    // On éteint toutes les LEDs au démarrage
    Serial.begin(9600);
    feux.eteindreTout();
}

void loop() {
    // Simule un cycle de feu tricolore

    // Allume la LED verte pendant 5 secondes
    feux.allumer(pinVerte);
    Serial.println("vert");
    delay(5000);
    feux.eteindre(pinVerte);

    // Allume la LED jaune pendant 2 secondes
    feux.allumer(pinJaune);
    Serial.println("Jaune");
    delay(2000);
    feux.eteindre(pinJaune);

    // Allume la LED rouge pendant 5 secondes
    feux.allumer(pinRouge);
    Serial.println("Rouge");
    delay(5000);
    feux.eteindre(pinRouge);
}

