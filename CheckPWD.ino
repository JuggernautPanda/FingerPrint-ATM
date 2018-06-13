#include<EEPROM.h>
char passwd[4];
char password[4];
char pass[4],pass1[4];
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
    for(int j=0;j<4;j++)
    pass[j]=EEPROM.read(j);
    Serial.println(pass);
    delay(5000);
}
