#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>

//========================== Pinout ==========================
#define RX 10
#define TX 11

#define BOUT1 2
#define LED1 3

#define BOUT2 4
#define LED2 5

#define BOUT3 6
#define LED3 7

#define BOUT4 12
#define LED4 13

//======================== Modifiable ========================
#define CODE 1989
#define COOLDOWN 750
//============================================================

SoftwareSerial soundSerial(RX, TX);
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);
unsigned long lastTime;



void setup()
{
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
  
  myDFPlayer.volume(10);  // 0 à 30
  myDFPlayer.play(1); // Lancement du premier fichier
  lastTime = millis();
}

void loop()
{
  digitalWrite(LED1, digitalRead(BOUT1));
  digitalWrite(LED2, digitalRead(BOUT2));
  digitalWrite(LED3, digitalRead(BOUT3));
  digitalWrite(LED4, digitalRead(BOUT4));

  if(millis() - lastTime > COOLDOWN) {
    if(digitalRead(BOUT1) == HIGH) {
      myDFPlayer.play(1);
      lastTime = millis();
    }
    if(digitalRead(BOUT2) == HIGH) {
      myDFPlayer.play(2);
      lastTime = millis();
    }
    if(digitalRead(BOUT3) == HIGH) {
      myDFPlayer.play(3);
      lastTime = millis();
    }
    if(digitalRead(BOUT4) == HIGH) {
      myDFPlayer.play(4);
      lastTime = millis();
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
