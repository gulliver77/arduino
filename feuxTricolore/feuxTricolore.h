// feuxTricolore.h

#ifndef FEUXTRICOLORE_H
#define FEUXTRICOLORE_H

#include <Arduino.h>

class FeuxTricolore {
private:
    int pinVerte;
    int pinJaune;
    int pinRouge;

public:
    FeuxTricolore(int pinVerte, int pinJaune, int pinRouge);
    void allumer(int pin);
    void eteindre(int pin);
    void eteindreTout();
};

#endif

