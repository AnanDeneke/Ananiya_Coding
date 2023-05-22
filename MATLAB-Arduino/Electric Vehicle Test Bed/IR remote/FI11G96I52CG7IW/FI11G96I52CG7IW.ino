#include <IRremote.h> // use the library for IR
 
const int receiver = 11; // pin 1 of IR receiver to Arduino digital pin 11
const int ledPin =  4;
const int forward = 7;
int Led = 4;
int fadeValue;
int motorSpeed = 0;
int lastCounter = 1;
int counter;
 
IRrecv irrecv(receiver); // create instance of 'irrecv'
 
decode_results results;
 
void setup()
 
{ 
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(forward,OUTPUT);
  irrecv.enableIRIn();
}
 
void loop()
{
  counter = lastCounter; 
  if (irrecv.decode(&results))
  {
    if (results.value == 16724175) // Button 1 pressed 
    {
      counter ++;
    }
    if (results.value == 16718055) // Button 2 pressed
    {
      counter --;
    }
  irrecv.resume();  
  }
   
   if (counter == 5)
      {
        digitalWrite( Led , HIGH);
        delay(500);
        digitalWrite( Led , LOW);
        delay(100);
      }
if (counter > 5){      //maximum for counter = 5
    counter = 1;
  }
   
  if (counter < 2){    //minimum for counter = 1
    counter = 1;
  }
 
  switch (counter){    //depending on the counter the fadevalue is sent to the led and the motor speed
   
  case 1:
  fadeValue = 00;
  break;
   
  case 2:
  fadeValue = 50;
  break;
   
  case 3:
  fadeValue = 120;
  break;
   
  case 4:
  fadeValue = 185;
  break;
   
  case 5:
  fadeValue = 255;
  break;
   
  }
  if( results.value == 16743045 ) {
    analogWrite(forward, motorSpeed);
    motorSpeed = motorSpeed + 25.5;         //ten percent increments 
    delay(500);
  }
  analogWrite(ledPin , fadeValue);    //set led with PWM value
  lastCounter=counter;
  
}
/*#include <IRremote.h>
#include <IRremoteInt.h>
IRrecv irrecv(11);
decode_results results;
long lastPressTime = 0;
int state = LOW;
void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}
void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value);
    irrecv.resume(); // Receive the next value
  }
}*/
