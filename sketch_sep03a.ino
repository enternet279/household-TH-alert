#include <Arduino.h>
#include "DHT.h"
#include <SPI.h>
#define DHTPIN 2
#define DHTTYPE DHT11   // 只留DHT 11
DHT dht(DHTPIN, DHTTYPE);
int H;
int HH;
int TT;
int T;
int speakerPin = 3;

int tempo = 290;
void set7Seg(int number, int pos);
char notegood[] = "cegC";
unsigned long beatsgood[] = {2, 2, 2, 2};
int lengthgood = sizeof(notegood);


const int pinState[9][7] = {
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 1, 1, 0}  // 9
};

void setup() {
  dht.begin();
  for (int i = 3; i < 14; i++) {
    pinMode(i, OUTPUT);
  }
}

void loop() {
  float h = dht.readHumidity();
  h = round(h * 100) / 100;
  float t = dht.readTemperature();
  t = round(t * 100) / 100;
  H = h;
  T = t;
  set7Seg(T / 10, 4);
  set7Seg(T % 10, 3);
  set7Seg(H / 10, 2);
  set7Seg(H % 10, 1);

  if (T / 10 > 3 || H / 10 > 8 || H / 10 < 4)
  { for (int i = 0; i < lengthgood; i++) {
      if (notegood[i] == ' ') {
        delay(beatsgood[i] * tempo);
      }
      else {
        playNote(speakerPin, notegood[i], beatsgood[i] * tempo);
      }
      delay(tempo / 10);
    }
  }
}

void set7Seg(int number, int pos) {
  for (int i = 10; i < 14; i++) {
    digitalWrite(i, 1);
  }

  for (int i = 0; i < 7; i++) {
    digitalWrite(i + 3, pinState[number - 1][i]);
  }

  digitalWrite(pos + 9, 0);
  delay(5);
}
void playNote(int OutputPin, char note, unsigned long duration) {
  char names[] = { 'n', 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 247, 261, 294, 330, 349, 392, 440, 494, 523 };
  for (int i = 0; i < 9; i++) {
    if (names[i] == note) {
      tone(OutputPin, tones[i], duration);
      delay(duration);
      noTone(OutputPin);
    }
  }
}
