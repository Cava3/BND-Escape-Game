#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>
#include <math.h>

//======================== Structure ========================

#define RED    1
#define BLUE   2
#define YELLOW 3
#define GREEN  4

struct BoutonLED {
  uint8_t boutPin;
  uint8_t LEDPin;
  unsigned char color;
  int code;
};


//========================== Pinout ==========================
#define RX 9
#define TX 10

//======================== Modifiable ========================
#define NCODE 1989
#define COOLDOWN 600
const char CCODE[4] {BLUE, RED, YELLOW, GREEN};
//============================================================

SoftwareSerial soundSerial(RX, TX);
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);
unsigned long lastTime;

BoutonLED simon[4];

void setup()
{
  BoutonLED simon[4] = {
  // PIN, LED, COLOR, ASSOCIATED NUMBER
    { 2,  3, RED,    (NCODE%10000)/1000}, //%10_000 comme safeguard
    { 4,  5, BLUE,   (NCODE%1000)/100},
    { 6,  7, YELLOW, (NCODE%100)/10},
    {12, 13, GREEN,  (NCODE%10)/1}
  };

  for(int bouton_i=0; bouton_i<4; bouton_i++){
    for(int ccode_i=0; ccode_i<4; ccode_i++){
      if(simon[bouton_i].color == CCODE[ccode_i])
        simon[bouton_i].code = (NCODE% (int) pow(10, 4-ccode_i)/ (int) pow(10, 3-ccode_i));
    }

    pinMode(simon[bouton_i].boutPin, INPUT_PULLUP);
    pinMode(simon[bouton_i].LEDPin,  OUTPUT);
  }

  soundSerial.begin(9600);
  Serial.begin(115200);

  Serial.println();
  Serial.println(F("Initialisation du module..."));
  
  if (!myDFPlayer.begin(soundSerial, true, true)) { // Initialisation du DFPlayer
    Serial.println(F("Démarage du module impossible. Vérifier la carte et les connections."));
    while(true)
      delay(0); // Blocage si non initialisé
    
  }
  Serial.println(F("DFPlayer Mini prêt."));
  
  myDFPlayer.volume(15);  // 0 à 30
  myDFPlayer.playMp3Folder(10); // Lancement du premier fichier
  lastTime = millis();
}

void loop()
{
  // On inverse tous les HIGH et LOW car on est en INPUT_PULLUP (PULLDOWN n'existe pas sur Arduino Uno)
  // Les LEDs réagissent en temps réel aux appuis sur les boutons
  for(int bouton_i=0 ; bouton_i < 4 ; bouton_i++)
    digitalWrite(simon[bouton_i].LEDPin, HIGH - digitalRead(simon[bouton_i].boutPin));

  // Check des appuis pour sons
  if(millis() - lastTime > COOLDOWN) {
    for(int bouton_i=0 ; bouton_i < 4 ; bouton_i++){
      if(digitalRead(simon[bouton_i].boutPin) == LOW) {
        myDFPlayer.playMp3Folder(simon[bouton_i].code);
        lastTime = millis();
        break;
      }
    }
  }
  
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); // Affichage détaillé du status du DFPlayer
  }
}

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
