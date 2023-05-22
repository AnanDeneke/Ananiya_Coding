
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9          // Configurable, see typical pin layout above
#define SS_PIN  53         

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
const int buzzer = 42;
void setup() {
	Serial.begin(9600);	
	SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();		// Init MFRC522
  pinMode(buzzer, OUTPUT);
}

void loop() {
  String content= "";
  byte letter;
	if ( ! mfrc522.PICC_IsNewCardPresent()) { // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
		return;
	}
  if ( ! mfrc522.PICC_ReadCardSerial()) {     //select one of the cards
		return;
	}
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "4B 9B E2 4C"){
    Serial.println("Authorized access");
    Serial.println();
    tone(buzzer, 5000);
    delay(1000);
    noTone(buzzer); 
    delay(3000);
  }
 
 else {
    Serial.println(" Access denied");
    tone(buzzer, 1000);
    delay(1000);
    noTone(buzzer); 
    delay(3000);
  }
} 
