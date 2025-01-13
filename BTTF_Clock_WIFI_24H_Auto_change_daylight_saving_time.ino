#include "Arduino.h"
#include "TM1637Display.h"
#include "WiFiManager.h"
#include "NTPClient.h"


// Module connection pins (Digital Pins)
#define red_CLK 13
#define red1_DIO 14
#define red2_DIO 15
#define red3_DIO 16

#define green_CLK 17
#define green1_DIO 18
#define green2_DIO 19
#define green3_DIO 21

#define orange_CLK 22
#define orange1_DIO 23
#define orange2_DIO 25
#define orange3_DIO 26

#define greenAM 32
#define greenPM 33

bool res;
//========================USEFUL VARIABLES=============================
int UTC = 2; // UTC = value in hour (SUMMER TIME) [For example: Paris UTC+2 => UTC=2]
const long utcOffsetInSeconds = 3600; // Offset in second
int Display_backlight = 255;

//Set the red displays
int red_day = 19;
int red_month = 09;
int red_year = 2091;
int red_hour = 11;
int red_minute = 05;

//set the orange displays
int orange_day = 19;
int orange_month = 09;
int orange_year = 1991;
int orange_hour = 11;
int orange_minute = 05;

//=====================================================================

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds*UTC);

TM1637Display green1(green_CLK, green1_DIO);
TM1637Display green2(green_CLK, green2_DIO);
TM1637Display green3(green_CLK, green3_DIO);

TM1637Display red1(red_CLK, red1_DIO);
TM1637Display red2(red_CLK, red2_DIO);
TM1637Display red3(red_CLK, red3_DIO);

TM1637Display orange1(orange_CLK, orange1_DIO);
TM1637Display orange2(orange_CLK, orange2_DIO);
TM1637Display orange3(orange_CLK, orange3_DIO);

void setup()
{ 
  pinMode(32, OUTPUT);
  pinMode(33, OUTPUT);
  Serial.begin(115200);
  
    WiFiManager manager;    
     
   manager.setTimeout(180);
  //fetches ssid and password and tries to connect, if connections succeeds it starts an access point with the name called "BTTF_CLOCK" and waits in a blocking loop for configuration
  res = manager.autoConnect("BTTF_CLOCK","password");
  
  if(!res) {
  Serial.println("failed to connect and timeout occurred");
  ESP.restart(); //reset and try again
  }
  

  delay(3000);


  timeClient.begin();
  red1.setBrightness(Display_backlight);
  red2.setBrightness(Display_backlight);
  red3.setBrightness(Display_backlight);
  
  green1.setBrightness(Display_backlight);
  green2.setBrightness(Display_backlight);
  green3.setBrightness(Display_backlight);

  orange1.setBrightness(Display_backlight);
  orange2.setBrightness(Display_backlight);
  orange3.setBrightness(Display_backlight);

}

void loop()
{
  timeClient.update();
  Serial.print("Time: ");
  Serial.println(timeClient.getFormattedTime());
  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime); 
  int currentYear = ptm->tm_year+1900;
  Serial.print("Year: ");
  Serial.println(currentYear);
  
  int monthDay = ptm->tm_mday;
  Serial.print("Month day: ");
  Serial.println(monthDay);

  int currentMonth = ptm->tm_mon+1;
  Serial.print("Month: ");
  Serial.println(currentMonth);
  
red1.showNumberDecEx(red_month,0b01000000,true,2,0);
red1.showNumberDecEx(red_day,0b01000000,true,2,2);
red2.showNumberDecEx(red_year,0b00000000,true);
red3.showNumberDecEx(red_hour,0b01000000,true,2,0);
red3.showNumberDecEx(red_minute,0b01000000,true,2,2);


green1.showNumberDecEx(currentMonth,0b01000000,true,2,0);
green1.showNumberDecEx(monthDay,0b01000000,true,2,2);
green2.showNumberDecEx(currentYear,0b00000000,true);
green3.showNumberDecEx(timeClient.getHours(),0b01000000,true,2,0);
green3.showNumberDecEx(timeClient.getMinutes(),0b01000000,true,2,2);


orange1.showNumberDecEx(orange_month,0b01000000,true,2,0);
orange1.showNumberDecEx(orange_day,0b01000000,true,2,2);
orange2.showNumberDecEx(orange_year,0b00000000,true);
orange3.showNumberDecEx(orange_hour,0b01000000,true,2,0);
orange3.showNumberDecEx(orange_minute,0b01000000,true,2,2);

if((currentMonth*30 + monthDay) >= 121 && (currentMonth*30 + monthDay) < 331){
timeClient.setTimeOffset(utcOffsetInSeconds*UTC);} // Change daylight saving time - Summer
else {timeClient.setTimeOffset((utcOffsetInSeconds*UTC) - 3600);} // Change daylight saving time - Winter


if(timeClient.getHours()>=13){
  digitalWrite(greenAM,0);
  digitalWrite(greenPM,1);}
  
else if(timeClient.getHours()==12){
  digitalWrite(greenAM,0);
  digitalWrite(greenPM,1);}

else{
  digitalWrite(greenAM,1);
  digitalWrite(greenPM,0);}

delay(20);

}
