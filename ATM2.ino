#include <Adafruit_Fingerprint.h>
#if ARDUINO >= 100
#include <SoftwareSerial.h>
#else
#include <NewSoftSerial.h>
#endif
#define buzzer A7
#include <Password.h>
#include<LiquidCrystal.h>
#include<EEPROM.h>
LiquidCrystal lcd(7,8,9,10,11,12);
#include <Keypad.h>
char passwd[4];
char pass[4],pass1[4];
int i=0;
int k=0;
int count = 0;
char customKey=0;

int getFingerprintIDez();
int led = 13;
int p = 0;
int z = 0;
int c1=0;
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
#if ARDUINO >= 100
SoftwareSerial mySerial(2, 3);
#else
NewSoftSerial mySerial(2, 3);
#endif

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);


Password password = Password( "1359" );

const byte ROWS = 4; // Four rows
const byte COLS = 4; // Three columns
// Define the Keymap
char keys[ROWS][COLS] = {
 {'1','2','3'},
 {'4','5','6'},
 {'7','8','9'},
 {'*','0','#'}
};


char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = { 4, A5, A4, A3 };// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = { A2, A1, A0 };


// Create the Keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
 lcd.begin(16,2);
 pinMode(6,OUTPUT);
 pinMode(5,OUTPUT);
 pinMode(13,OUTPUT);
 pinMode(buzzer,OUTPUT);
 Serial.begin(9600);
 keypad.addEventListener(keypadEvent); //add an event listener for this keypad
 keypad.setDebounceTime(100);
 lcd.print("  ABC Bank ");
 lcd.setCursor(0,1);
 lcd.print("  ATM ");
 delay(2000);
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Enter Amount");
 lcd.setCursor(0,1);
 lcd.print("and #:");
 //lcd.setCursor(0,1);
 //for(int j=0;j<4;j++)
  //EEPROM.write(j, j+49);
  for(int j=0;j<4;j++)
  pass[j]=EEPROM.read(j);
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
       case '#': z = guessPassword(); break;
       case '*': starPressed(z); break;
        default:
              count++;
              password.append(eKey);
              passwd[i++]=eKey;
              lcd.print(eKey);
              
 }
 
 }}

int guessPassword(){
    Serial.print("Processing... ");
    if (count == 3)
    {
      count = 0;
      lcd.clear();
      lcd.print("Amount entered");
      delay(2000);
      lcd.clear();
      lcd.print("Now enter Acc");
      lcd.setCursor(0,1);
      lcd.print("U ID & #:");
      Serial.println("Validating Account ");
      i=0;
    }
    if (count == 4)
    {
      count = 0;
    for(int j=0;j<count;j++)
    pass[j]=EEPROM.read(j);
    
    
    if(!(strncmp(passwd, pass,4)))
    {
      lcd.clear();
      /*lcd.print("Valid Press *");
      lcd.setCursor(0,1);
      lcd.print("and fingerprint");
      Serial.println("Valid Account ");*/
      lcd.print("* &  finger");
      lcd.setCursor(0,1);
      digitalWrite(led, HIGH);
      beep();
      Serial.print("Now press # for amount:");
      i=0;
      digitalWrite(led, LOW);
       digitalWrite(5, HIGH);
       delay(1000);
       digitalWrite(6, HIGH);
       delay(1000);
       digitalWrite(6, LOW);
       delay(2500);
       digitalWrite(5, LOW);
       return 1;
    }
    else
    {
     
     // digitalWrite(buzzer, HIGH);
      lcd.clear();
      lcd.print("Access Denied...");
      lcd.setCursor(0,1);
      lcd.print("Try again");
      delay(2000);
      lcd.clear();
      lcd.print("Enter Acc:");
      lcd.setCursor(0,1);
      i=0;
    //  digitalWrite(buzzer, LOW); 
     /* lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Access Denied...");
      delay(2000);
      lcd.setCursor(0,1);
      lcd.clear();
      lcd.print("Try again");
      delay(2000);
      lcd.clear();
      lcd.print("Enter Acc No");
      delay(2000);
      lcd.clear();
      //lcd.setCursor(0,0);
      //delay(2000);
      //lcd.clear();
      i=0;
      Serial.println("INVALID Account No ");
      digitalWrite(buzzer, HIGH);
      
      
      digitalWrite(buzzer, LOW); */
      return 0;
      }
    }
  /*  if (password.evaluate()){           Serial.println("VALID PASSWORD "); //
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
    } */

    //return 1;
}

void starPressed(int t){
      
     // set the data rate for the sensor serial port
     if (t)
     {

      finger.begin(57600);
 
 
 if (finger.verifyPassword()) {
   Serial.println("Found fingerprint sensor!");
 } else {
   Serial.println("Did not find fingerprint sensor :(");
   while (1);
 }
 
 //lcd.setCursor(0,1);
// lcd.print("Now press * for authentication:");
 Serial.println("Waiting for valid finger...");
 unsigned long timeNow = millis();
 while (millis() - 20000 < timeNow) getFingerprintIDez();
}
else
{
  Serial.println("Enter valid account");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Invalid account");
      delay(2000);
      lcd.setCursor(0,1);
      lcd.print("Press No and #");
  }
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
 //lcd.clear();
// lcd.setCursor(0,0);
 //lcd.print("Processing");
 uint8_t p = finger.getImage();
 if (p != FINGERPRINT_OK)  return -1;

 p = finger.image2Tz();
 if (p != FINGERPRINT_OK)  return -1;

 p = finger.fingerFastSearch();
 if (p != FINGERPRINT_OK)  return -1;

 //lcd.clear();
// lcd.setCursor(0,0);
 //lcd.print("Collect your cash");
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
 //delay(5000);
 beep();
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Collect Cash");
 lcd.setCursor(0,1);
 lcd.print("Enter Acc:");
 return finger.fingerID;
} 

void beep()
{
  digitalWrite(buzzer, HIGH);
  delay(2000);
  digitalWrite(buzzer, LOW);
}
