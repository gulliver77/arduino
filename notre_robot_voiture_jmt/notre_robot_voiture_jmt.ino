/*// parti du programme 3.0 évolué en cpp
  //www.elegoo.com

  //    Left motor truth table
  //  ENA         IN1               IN2         Description
  //  LOW   Not Applicable    Not Applicable    Motor is off
  //  HIGH        LOW               LOW         Motor is stopped (brakes)
  //  HIGH        HIGH              LOW         Motor is on and turning forwards
  //  HIGH        LOW               HIGH        Motor is on and turning backwards
  //  HIGH        HIGH              HIGH        Motor is stopped (brakes)

  //    Right motor truth table
  //  ENB         IN3             IN4         Description
  //  LOW   Not Applicable   Not Applicable   Motor is off
  //  HIGH        LOW             LOW         Motor is stopped (brakes)
  //  HIGH        LOW             HIGH        Motor is on and turning forwards
  //  HIGH        HIGH            LOW         Motor is on and turning backwards
  //  HIGH        HIGH            HIGH        Motor is stopped (brakes)

  //    The direction of the car's movement
  //  Left motor    Right motor     Description
  //  stop(off)     stop(off)       Car is stopped
  //  forward       forward         Car is running forwards
  //  forward       backward        Car is turning right
  //  backward      forward         Car is turning left
  //  backward      backward        Car is running backwards
*/
#include "ControleMoteur.h"
#include <SoftwareSerial.h>

const byte rxPin = 14;  //entrant A0
const byte txPin = 15;  //sortant A1

SoftwareSerial SerialCamera(rxPin, txPin);  //SoftwareSerial(rxPin, txPin, inverse_logic)

extern String receptionCam;
char caracterRecu;

void setup() {
  Serial.begin(115200);
  SerialCamera.begin(9600);

  InitPin();
}

void loop() {

  // read from port 1, send to port 0:
  if (SerialCamera.available()) {
    caracterRecu = SerialCamera.read();
    if (caracterRecu == '\n') {
      traitementBouton();

    } else {
      receptionCam += caracterRecu;
    }
  }

}

