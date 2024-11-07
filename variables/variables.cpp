#include "variables.h"

void afficherEtat() {
    if (etatSysteme) {
        Serial.println("Système actif");
    } else {
        Serial.println("Système inactif");
    }
}

