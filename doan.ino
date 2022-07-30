// Dự án: Hệ thống cảm biến khí gas
// Tác giả: Lê Đức Nhân
// Trường: THPT Chuyên Nguyễn Du
// Người hỗ trợ: Võ Tấn Tuyên
// Năm học: 2022 - 2023
#include <LiquidCrystal_I2C.h>
            LiquidCrystal_I2C lcd(0x27,16,02);
#include "SIM900.h"
#include <SoftwareSerial.h>
SoftwareSerial sim(10, 11);                             // 10 - Tx, 11 - Rx
String ReadBuffer = "";
String number1 = "0855223004";                          // thay số điện thoại tại đây
int _timeout;
String _buffer;
int t=0;
const int sensor = A0;                                  // chân ra cảm biến
const int led = 4;                                      // chân ra led hoặc điều khiển mosfet
int sensorValue = 0; 
int settingvalue = 800;                                 // giá trị cài đặt cảnh báo
int i = 0;

void setup()
{ 
  Serial.begin(9600);                                   //19200
  sim.begin(9600);                                      //19200
  pinMode(led, OUTPUT);
  lcd.init();                                           // initialize the lcd  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("Vo Tan Tuyen");
  lcd.setCursor(1,1);
  lcd.print("Le Duc Nhan");
}

void loop() 
{
sensorValue = analogRead(sensor);
Serial.println(sensorValue);

if(sensorValue >=800){
  
digitalWrite(led, HIGH);
call();

for (i = 1; i<=60; i = i + 1) {                         //đếm 60s , trong khi 60s này nó vừa đủ gọi, hết 60s nó sẽ thực hiện bước tt là cúp máy
    Serial.println(i);
    delay(1000);
  }
    sim.println("ATH");                                 // thực hiện lệnh cúp máy 
    Serial.println("cup may chuyen sang gui tin nhan"); 
    message();
    delay(5000);
    Serial.println("da gui tin nhan"); 
}

if(sensorValue < 800)
{
digitalWrite(led, LOW);
}
//test();
}

void call()
{                      
  sim.print (F("ATD"));                                 //cú pháp gọi: ATD 0855223004 ;\r\n
  sim.print (number1);
  sim.print (F(";\r\n"));
  _buffer = _readSerial();
  Serial.println(_buffer);
}

String _readSerial() {
  _timeout = 0;
  while  (!sim.available() && _timeout < 10000  )
  {
    delay(13);
    _timeout++;
  }
  if (sim.available()) 
  {
    return sim.readString();
  }                                       
}

void message()
{  
  sim.println("AT+CMGF=1");    
  delay(1000);
  Serial.println ("Set SMS Number");
  sim.println("AT+CMGS=\"" + number1 + "\"\r"); 
  delay(1000);
  String SMS = "co khi doc";
  sim.println(SMS);
  delay(100);
  sim.println((char)26);                                // ASCII code of CTRL+Z
  delay(1000);
  _buffer = _readSerial();
}