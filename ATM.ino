#include <Adafruit_Fingerprint.h>
#if ARDUINO >= 100
#include <SoftwareSerial.h>
#else
#include <NewSoftSerial.h>
#endif

#include <Password.h>

#include <Keypad.h>

int getFingerprintIDez();
int led = 13;

// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
#if ARDUINO >= 100
SoftwareSerial mySerial(2, 3);
#else
NewSoftSerial mySerial(2, 3);
#endif

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);


Password password = Password( "1234" );

const byte ROWS = 4; // Four rows
const byte COLS = 3; // Three columns
// Define the Keymap
char keys[ROWS][COLS] = {
 {'1','2','3'},
 {'4','5','6'},
 {'7','8','9'},
 {'*','0','#'}
};
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = { 4, A5, A4, A3 };// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = { A2, A1, A0 };


// Create the Keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){

 pinMode(6,OUTPUT);
 pinMode(5,OUTPUT);
 pinMode(13,OUTPUT);
 Serial.begin(9600);
 keypad.addEventListener(keypadEvent); //add an event listener for this keypad
 keypad.setDebounceTime(100);
}

void loop(){
 keypad.getKey();

}

//take care of some special events
void keypadEvent(KeypadEvent eKey){
 switch (keypad.getState()){
   case PRESSED:
     Serial.print("Pressed: ");
     Serial.println(eKey);
     switch (eKey){
       case '#': guessPassword(); break;
       case '*': starPressed(); break;
        default:
              password.append(eKey);
 }
 
 }}

void guessPassword(){
    Serial.print("Guessing password... ");
    if (password.evaluate()){           Serial.println("VALID PASSWORD "); //
          digitalWrite(5, HIGH);
            delay(1000);
          digitalWrite(6, HIGH);
            delay(1000);
          digitalWrite(6, LOW);
            delay(25000);
          digitalWrite(5, LOW);
        
             password.reset(); //resets password after correct entry
    }else{
          Serial.println("INVALID PASSWORD ");
             password.reset(); //resets password after INCORRECT entry
    } 
}

void starPressed(){
     // set the data rate for the sensor serial port
 finger.begin(57600);
 
 
 if (finger.verifyPassword()) {
   Serial.println("Found fingerprint sensor!");
 } else {
   Serial.println("Did not find fingerprint sensor :(");
   while (1);
 }
 Serial.println("Waiting for valid finger...");
 unsigned long timeNow = millis();
 while (millis() - 20000 < timeNow) getFingerprintIDez();
}
uint8_t getFingerprintID() {
 uint8_t p = finger.getImage();
 switch (p) {
   case FINGERPRINT_OK:
     Serial.println("Image taken");
     break;
   case FINGERPRINT_NOFINGER:
     Serial.println("No finger detected");
     return p;
   case FINGERPRINT_PACKETRECIEVEERR:
     Serial.println("Communication error");
     return p;
   case FINGERPRINT_IMAGEFAIL:
     Serial.println("Imaging error");
     return p;
   default:
     Serial.println("Unknown error");
     return p;
 }

 // OK success!

 p = finger.image2Tz();
 switch (p) {
   case FINGERPRINT_OK:
     Serial.println("Image converted");
     break;
   case FINGERPRINT_IMAGEMESS:
     Serial.println("Image too messy");
     return p;
   case FINGERPRINT_PACKETRECIEVEERR:
     Serial.println("Communication error");
     return p;
   case FINGERPRINT_FEATUREFAIL:
     Serial.println("Could not find fingerprint features");
     return p;
   case FINGERPRINT_INVALIDIMAGE:
     Serial.println("Could not find fingerprint features");
     return p;
   default:
     Serial.println("Unknown error");
     return p;
 }
 
 // OK converted!
 p = finger.fingerFastSearch();
 if (p == FINGERPRINT_OK) {
   Serial.println("Found a print match!");
 } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
   Serial.println("Communication error");
   return p;
 } else if (p == FINGERPRINT_NOTFOUND) {
   Serial.println("Did not find a match");
   return p;
 } else {
   Serial.println("Unknown error");
   return p;
 }   
 
 // found a match!
 Serial.print("Found ID #"); Serial.print(finger.fingerID); 
 Serial.print(" with confidence of "); Serial.println(finger.confidence); 
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
 uint8_t p = finger.getImage();
 if (p != FINGERPRINT_OK)  return -1;

 p = finger.image2Tz();
 if (p != FINGERPRINT_OK)  return -1;

 p = finger.fingerFastSearch();
 if (p != FINGERPRINT_OK)  return -1;
 
 // found a match!
 digitalWrite(5, HIGH);
 digitalWrite(13, HIGH);
 delay(1000);
 digitalWrite(6, HIGH);
 delay(500);
 digitalWrite(6, LOW);
 delay(25000);
 digitalWrite(5, LOW);
 digitalWrite(13, LOW);

 Serial.print("Found ID #"); Serial.print(finger.fingerID); 
 Serial.print(" with confidence of "); Serial.println(finger.confidence);
 return finger.fingerID;
} 

