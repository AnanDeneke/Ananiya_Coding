#include <dht.h>        //library for DHT11 sensor
#define dht_apin A15
dht DHT;
const int trigPin = 9;
const int echoPin = 10;
int forward = 7;
const int backward = 5;
long duration;
int distance;
int LED1 = 2;
int LED2 = 3;
int LED3 = 4;
void setup() {
  digitalWrite(LED1,HIGH);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
  delay(500);
  pinMode(forward,OUTPUT);
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT); 
  pinMode(backward,OUTPUT);
}
void loop() {
  DHT.read11(dht_apin);  // read temperature and water sensor
  if( Serial.available()){
     Serial.println(DHT.humidity);
     Serial.println(DHT.temperature);
  }
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  Serial.println(distance);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  analogWrite(backward, 0);
  
  if( (distance >= 30)&& (distance < 60)){
   digitalWrite(LED1, HIGH);
   analogWrite(forward, 255);
  }
  if((distance > 7) && (distance <30)){
    digitalWrite(LED2, HIGH);
    digitalWrite(LED1, HIGH);
    analogWrite(forward, 120);
    
  }
  if( distance <= 7){
    digitalWrite(LED3, HIGH);
     digitalWrite(LED2, HIGH);
    digitalWrite(LED1, HIGH);
    analogWrite(forward, 0);
  }
  //delay(1000);
 
}
