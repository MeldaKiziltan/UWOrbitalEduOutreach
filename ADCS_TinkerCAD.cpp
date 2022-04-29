// C++ code
//

//libraries
#include <string.h>
#include <Adafruit_NeoPixel.h>
#define PIN  		7
#define NUMPIXELS   4
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//variables
const int tilt = 2;
const int LED = 13;
const int m1p1 = 3; //motor 1, pin 1
const int m1p2 = 4; //motor 1, pin 2
const int m2p1 = 5; //motor 2, pin 1
const int m2p2 = 6; //motor 2, pin 2
const int tempPin = A0;
const int lightSense = A1;

//colours
uint32_t red = strip.Color(255, 0, 0); //red
uint32_t orange = strip.Color(255, 100, 10); //orange
uint32_t yellow = strip.Color(255, 255, 0); //yellow
uint32_t green = strip.Color(0, 255, 0);//green

//setup
void setup()
{
  Serial.begin(9600); //serial monitor
  
  //Adafruit
  strip.begin();
  strip.setBrightness(100);
  strip.show();
  
  //Pins
  pinMode(tilt, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(m1p1, OUTPUT);
  pinMode(m1p2, OUTPUT);
  pinMode(m2p1, OUTPUT);
  pinMode(m2p2, OUTPUT);
  pinMode(tempPin, INPUT);
  pinMode(lightSense, INPUT);
}

//loop
void loop()
{
  // int temp = map(((analogRead(tempPin) - 20) * 3.04), 0, 1023, -40, 125);
  // Serial.println(digitalRead(2));
  Serial.println(analogRead(lightSense));
  
  while(analogRead(lightSense)){
    battLvl();
    if(digitalRead(tilt) == 0){
      while (digitalRead(tilt) == 0){
        Serial.println("Tilt adjustment");
        digitalWrite(LED, LOW);
        roll(m1p1, m1p2, m2p1, m2p2);
      }
      stopRoll(m1p1, m1p2, m2p1, m2p2);
    } else if (highTemp() == true){
      while (highTemp() == true){
        Serial.println("Too hot! Rolling");
        roll(m1p1, m1p2, m2p2, m2p1);
      }
      stopRoll(m1p1, m1p2, m2p1, m2p2);
    } else {
      digitalWrite(LED, HIGH);
    }
  }
  
}

//functions
//void function, takes in two parameters of type int
void roll(int mp1, int mp2, int mp3, int mp4){
  digitalWrite(mp1, HIGH);
  digitalWrite(mp2, LOW);
  digitalWrite(mp3, HIGH);
  digitalWrite(mp4, LOW);
}

void stopRoll(int mp1, int mp2, int mp3, int mp4){
  digitalWrite(mp1, LOW);
  digitalWrite(mp2, LOW);
  digitalWrite(mp3, LOW);
  digitalWrite(mp4, LOW);
}

bool highTemp(){
  int temp = map(((analogRead(tempPin) - 20) * 3.04), 0, 1023, -40, 125);
  //Serial.println((String) temp + "*C"); //using the string library
  if(temp >= 50){
    return true;
  }else {
    return false;
  }
}

void battLvl(){
  int charge = analogRead(lightSense);
  Serial.println(charge);
  strip.clear();
  
  if (charge <= 720){
    strip.setPixelColor(0, red);
  } else if ((charge > 720) && (charge <= 860)) {
    for (int i = 0; i < NUMPIXELS - 2; i++){
      strip.setPixelColor(i, orange);
    }
  } else if ((charge > 860)&&(charge <= 900)){
    for (int i = 0; i < NUMPIXELS - 1; i++){
      strip.setPixelColor(i, yellow);
    }
  } else {
    for (int i = 0; i < NUMPIXELS; i++){
      strip.setPixelColor(i, green);
    }
  }
  strip.show();
}
