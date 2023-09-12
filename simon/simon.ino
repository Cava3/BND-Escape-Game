#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>

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
#define COOLDOWN 500
//============================================================


SoftwareSerial mySerial( RX , TX );
DFRobotDFPlayerMini myDFPlayer;
unsigned long int lastPlayed;

void setup() {

  pinMode(BOUT1, INPUT);
  pinMode(LED1, OUTPUT);

  pinMode(BOUT2, INPUT);
  pinMode(LED2, OUTPUT);

  pinMode(BOUT3, INPUT);
  pinMode(LED3, OUTPUT);
  
  pinMode(BOUT4, INPUT);
  pinMode(LED4, OUTPUT);


  // Lancement des Serial pour la musique (mySerial) et pour le port série (Serial)
  mySerial.begin(9600);
  Serial.begin(115200);

  // Préparation du module de son
  if(!myDFPlayer.begin(mySerial, true, false)) {
    Serial.println("Module non-fonctionnel. Vérifier la carte et les connection.");
    while (true)
      delay(0); // Bloquage si module non-fonctionnel
  }

  // Initialisation du jeu
  //lastPlayed = 0;

  myDFPlayer.volume(15);
  //delay(1000);
  for(int i=10;i>0;i--){
    Serial.println(i);
    //myDFPlayer.play(i);
    //delay(750);
  }
  
  //delay(10000);
}

void loop() {
  // Son à l'appui sur chaque bouton
  
  digitalWrite(LED1, digitalRead(BOUT1));
  digitalWrite(LED2, digitalRead(BOUT2));
  digitalWrite(LED3, digitalRead(BOUT3));
  digitalWrite(LED4, digitalRead(BOUT4));

  /**
  if(millis() - lastPlayed > COOLDOWN){
    if(digitalRead(BOUT1) == HIGH){
      myDFPlayer.play(BOUT2);
      lastPlayed = millis();
    }

    if(digitalRead(BOUT2) == HIGH) {
      myDFPlayer.play(BOUT2);
      lastPlayed = millis();
    }

    if(digitalRead(BOUT3) == HIGH) {
      myDFPlayer.play(BOUT3);
      lastPlayed = millis();
    }

    if(digitalRead(BOUT4) == HIGH) {
      myDFPlayer.play(BOUT4);
      lastPlayed = millis();
    }
  }
  */

}