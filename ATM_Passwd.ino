#include <Keypad.h>
#include<LiquidCrystal.h>
#include<EEPROM.h>
int led = 13;
#define buzzer 15
LiquidCrystal lcd(7,8,9,10,11,12);
int m11 = 5;
int m12 = 6;
char passwd[4];
char pass[4],pass1[4];
int i=0;
char customKey=0;

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = { 4, A5, A4, A3 };// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = { A2, A1, A0 };

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup()
{
  lcd.begin(16,2);
  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(m11, OUTPUT);
  pinMode(m12, OUTPUT);
  lcd.clear();
  lcd.print("  ATM  ");
  lcd.setCursor(0,1);
  lcd.print("  Account Reset ");
  delay(2000);
  lcd.clear();
  lcd.print("Enter Ur Acc:");
  lcd.setCursor(0,1);
  for(int j=0;j<4;j++)
  EEPROM.write(j, j+49);
  for(int j=0;j<4;j++)
  pass[j]=EEPROM.read(j);
}
  
void loop()
{
  customKey = customKeypad.getKey();
  if(customKey=='#')
  change();
  if (customKey)
  {
     passwd[i++]=customKey;
     lcd.print(customKey);
     beep();
  }
  if(i==4)
  {
    delay(200);
    for(int j=0;j<4;j++)
    pass[j]=EEPROM.read(j);
    if(!(strncmp(passwd, pass,4)))
    {
      digitalWrite(led, HIGH);
      beep();
      lcd.clear();
      lcd.print("Passkey Accepted");
      delay(2000);
      lcd.setCursor(0,1);
      lcd.print("#.Change Passkey");
      delay(2000);
      lcd.clear();
      lcd.print("Enter Passkey:");
      lcd.setCursor(0,1);
      i=0;
      digitalWrite(led, LOW);
    }
    
    else
    {
      digitalWrite(buzzer, HIGH);
      lcd.clear();
      lcd.print("Access Denied...");
      lcd.setCursor(0,1);
      lcd.print("#.Change Passkey");
      delay(2000);
      lcd.clear();
      lcd.print("Enter Passkey:");
      lcd.setCursor(0,1);
      i=0;
      digitalWrite(buzzer, LOW);
    }
  }
}

void change()
{
  int j=0;
  lcd.clear();
  lcd.print("UR Current Passk");
  lcd.setCursor(0,1);
  while(j<4)
  {
    char key=customKeypad.getKey();
    if(key)
    {
      pass1[j++]=key;
      lcd.print(key);
      beep();
    }
    key=0;
  }
  delay(500);
  
  if((strncmp(pass1, pass, 4)))
  {
    lcd.clear();
    lcd.print("Wrong Passkey...");
    lcd.setCursor(0,1);
    lcd.print("Better Luck Again");
    delay(1000);
  }
  else
  {
    j=0;
    
  lcd.clear();
  lcd.print("Enter New Passk:");
  lcd.setCursor(0,1);
  while(j<4)
  {
    char key=customKeypad.getKey();
    if(key)
    {
      pass[j]=key;
      lcd.print(key);
      EEPROM.write(j,key);
      j++;
      beep();
    }
  }
  lcd.print("  Done......");
  delay(1000);
  }
  lcd.clear();
  lcd.print("Enter Ur Passk:");
  lcd.setCursor(0,1);
  customKey=0;
}

void beep()
{
  digitalWrite(buzzer, HIGH);
  delay(20);
  digitalWrite(buzzer, LOW);
}
