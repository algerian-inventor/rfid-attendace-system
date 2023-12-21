#include <SPI.h>
#include <MFRC522.h>


#define SS_PIN 10 //RX slave select
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);
 
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

byte card_ID[4]; //card UID size 4byte
byte Name1[4]={0x93,0x37,0x21,0xB7};//first UID card
byte Name2[4]={0x43,0xEE,0xD1,0xA9};//second UID card

int NumbCard[2];//the number of cards. in my case i have just two cards.
int j=0;  
int statu[2];//the number of cards. in my case i have just two cards.
int s=0;        



String Log;
String Name;//user name
long Number;//user number
int n ;//The number of card you want to detect (optional)  

void setup() {
  lcd.init();                      // initialize the lcd 
  lcd.backlight();

  Serial.begin(9600); // Initialize serial communications with the PC
  SPI.begin();  // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card
  
  Serial.println("CLEARSHEET");                 // clears starting at row 1
  Serial.println("LABEL,Date,Name,Number,Time IN,Time OUT");// make four columns (Date,Time,[Name:"user name"]line 48 & 52,[Number:"user number"]line 49 & 53)

  
  lcd.setCursor(2,0);
  lcd.print("Good Morning");//Wlecome Message will appear in the beginning. 
  delay(200);
   }
   
void loop() {
  //look for new card
   if ( ! mfrc522.PICC_IsNewCardPresent()) {
  return;//got to start of loop if there is no card present
 }
 // Select one of the cards
 if ( ! mfrc522.PICC_ReadCardSerial()) {
  return;//if read card serial(0) returns 1, the uid struct contians the ID of the read card.
 }
 
 for (byte i = 0; i < mfrc522.uid.size; i++) {
     card_ID[i]=mfrc522.uid.uidByte[i];

       if(card_ID[i]==Name1[i]){
       Name="ABDRHMANE";//user name
       Number=3403798;//user number
       j=0;
       s=0;
      }
      else if(card_ID[i]==Name2[i]){
       Name="MOHAMED";//user name
       Number=3403799;//user number
       j=1;
       s=1;
      }
      else{
          
          goto cont;//go directly to line 71
     }
}
      if(NumbCard[j] == 1 && statu[s] == 0){
      statu[s]=1;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Good Bye");
      lcd.setCursor(0,1);
      lcd.print(Name);
      Serial.print("DATA,DATE," + Name);//send the Name to excel
      Serial.print(",");
      Serial.print(Number); //send the Number to excel
      Serial.print(",");
      Serial.print("");
      Serial.print(",");
      Serial.println("TIME");
      
      }
      else if(NumbCard[j] == 0){
      NumbCard[j] = 1;
      
      n++;//(optional)
      lcd.clear();
      lcd.setCursor(4,0);
      lcd.print("Welcome ");
      lcd.setCursor(0,1);
      lcd.print(Name);
      Serial.print("DATA,DATE," + Name);//send the Name to excel
      Serial.print(",");
      Serial.print(Number); //send the Number to excel
      Serial.print(",");
      Serial.print("TIME");
      Serial.print(",");
      Serial.println("");
      
      delay(30);
   
      }
      else if(statu[s] == 1){
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print(Name);
     lcd.setCursor(0,1);
     lcd.print("Already Left");
      }
      delay(1000);
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print("Good Morning");

cont:
delay(2000);


//if you want to close the Excel when all card had detected and save Excel file in Names Folder. in my case i have just 2 card (optional)
/*if(n==2){
    Serial.println("SAVEWORKBOOKAS,Names/WorkNames");
    Serial.println("FORCEEXCELQUIT");
    }*/
}
    
