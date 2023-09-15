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
#define RX 0
#define TX 1

//======================== Modifiable ========================
#define COOLDOWN 600
#define NCODE 1812
const char CCODE[4] {RED, YELLOW, BLUE, GREEN};
//============================================================

SoftwareSerial soundSerial(RX, TX);
DFRobotDFPlayerMini myDFPlayer;
void handleDetail(uint8_t type, int value);
unsigned long lastTime;
int res_state;
int playing;

BoutonLED simon[4] = {
  // PIN, LED, COLOR, ASSOCIATED NUMBER
    (BoutonLED) { 3,  4, RED,    0},
    (BoutonLED) { 6,  7, BLUE,   0},
    (BoutonLED) { 9,  10, YELLOW, 0},
    (BoutonLED) {12, 13, GREEN,  0}
  };

void setup()
{
  soundSerial.begin(9600);

  for(int bouton_i=0; bouton_i<4; bouton_i++){
    for(int ccode_i=0; ccode_i<4; ccode_i++){
      if(simon[bouton_i].color == CCODE[ccode_i]){
        simon[bouton_i].code = (int) (NCODE / pow(10, 3-ccode_i)) % 10; // (abcd / 10^2)%10 = b
      }
    }

    pinMode(simon[bouton_i].boutPin, INPUT_PULLUP);
    pinMode(simon[bouton_i].LEDPin,  OUTPUT);
  }


  // Serial.println();
  // Serial.println(F("Initialisation du module..."));
  
  if (!myDFPlayer.begin(soundSerial, true, true)) { // Initialisation du DFPlayer
    // Serial.println(F("Démarage du module impossible. Vérifier la carte et les connections."));
    while(true)
      delay(0); // Blocage si non initialisé
    
  }
  // Serial.println(F("DFPlayer Mini prêt."));
  
  myDFPlayer.volume(15);  // 0 à 30
  myDFPlayer.playMp3Folder(11); // Son lancement

  lastTime = millis();
  res_state = 0;
  playing = 0; // Valeur par defaut
}

void loop()
{
  // A la réussite, relire le code.
  // if(res_state >= 4){
  //   if(millis()-lastTime > COOLDOWN){
  //     myDFPlayer.playMp3Folder((int) (NCODE / pow(10, 3- (++playing))) % 10);
  //     lastTime = millis();
  //   }
  //   return;
  // }

  if(res_state >= 4 && millis()-lastTime > COOLDOWN){
    myDFPlayer.playMp3Folder(10); // Son réussite
    lastTime = millis();
    res_state = 0;
  }

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

        // Attention cas particulier du premier nombre
        if(simon[bouton_i].color != CCODE[res_state])
          res_state = 0;

        if(simon[bouton_i].color == CCODE[res_state])
          res_state++;

        break;
      }
    }
  }
  
  if (myDFPlayer.available()) {
    handleDetail(myDFPlayer.readType(), myDFPlayer.read()); // Affichage détaillé du status du DFPlayer
  }
}

void handleDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      // Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      // Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      // Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      // Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      // Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      // Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      // Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      // Serial.print(F("Number:"));
      // Serial.print(value);
      // Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      // Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          // Serial.println(F("Card not found"));
          break;
        case Sleeping:
          // Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          // Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          // Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          // Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          // Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          // Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
