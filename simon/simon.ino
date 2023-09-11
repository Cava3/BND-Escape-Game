#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

#define RX 0
#define TX 1

#define BOUT1 2 //PIN mais aussi NOM du fichier audio
#define BOUT2 3 //PIN mais aussi NOM du fichier audio
#define BOUT3 4 //PIN mais aussi NOM du fichier audio
#define BOUT4 5 //PIN mais aussi NOM du fichier audio

#define LED1 6
#define LED2 7
#define LED3 8
#define LED4 9

//======================== Modifiable ========================
#define CODE 1989
#define COOLDOWN 500

#define WIFI_SSID "rcpvp"
#define WIFI_PWD "J'aime le 94"
//============================================================


SoftwareSerial mySerial( RX , TX );
DFRobotDFPlayerMini myDFPlayer;
unsigned long int lastPlayed;

void setup() {
  // Lancement des Serial pour la musique (mySerial) et pour le port série (Serial)
  mySerial.begin(9600);
  Serial.begin(115200);

  // Préparation du module de son
  if(!myDFPlayer.begin(mySerial)) {
    Serial.println("Module non-fonctionnel. Vérifier la carte et les connection.");
    while (true); // Bloquage si module non-fonctionnel
  }

  // Préparation de la connexion
  myDFPlayer.volume(20);
  myDFPlayer.play("connexion.mp3"); // Supportés : .mp3 et .wav
  Serial.print("Connexion au réseau wifi");

  WiFi.begin(WIFI_SSID, WIFI_PWD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\nConnecté !\n\n");
  myDFPlayer.play("connecte.mp3");

  // Initialisation du jeu
  lastPlayed = 0;
}

void loop() {
  // Son à l'appui sur chaque bouton
  digitalWrite(LED1, digitalRead(BOUT1));
  digitalWrite(LED2, digitalRead(BOUT2));
  digitalWrite(LED3, digitalRead(BOUT3));
  digitalWrite(LED4, digitalRead(BOUT4));

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
}