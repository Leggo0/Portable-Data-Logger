/*
 * Written by Austin Salois
 * Summer 2019
 * For: Fairfield Osborn Preserve and Chris Halle
 * Purpose: Create a simple temperatre and humidity data logger
 * to be used when studying microclimates on the preserve. 
 * The logger should take accurate data readings, alone with
 * an accurate time of when the data was taken for future proccessing
 * 
 * https://github.com/Leggo0/Portable-Data-Logger
 */


//Includes
#include "SdFat.h"
#include <SPI.h>
#include "RTClib.h"
#include <DHT.h>
#include <DHT_U.h>

//Defines
#define DHTPIN 6
#define DHTTYPE DHT11
#define SD_CS_PIN 4
#define SDCARDLED 8


SdFat SD;
DHT dht(DHTPIN, DHTTYPE);

RTC_DS3231 rtc;



//Creating blank file name
String FILENAME;

const String header = "Date, Time(24Hr), Temp(C), Humidity"; //Header for file, does not change
void setup() 
{
  
  dht.begin();
  SD.begin(SD_CS_PIN);
  pinMode(SDCARDLED, OUTPUT);
  DateTime now = rtc.now(); //We need to get the data to create the file name
  int month =  now.month();
  int day = now.day();
  int hour = now.hour();
  int minute = now.minute();
  Serial.println("Data grabbed for file name creation");
  //Create the file name
  FILENAME = String(now.month()) +  "-" + String(now.day()) + "@" + String(now.hour()) + "-" + String(now.minute()) + ".csv";

  
  File dataFile = SD.open(FILENAME, FILE_WRITE); //Push header to file
  dataFile.println(header);
  dataFile.close();
  delay(2000);
}


void loop() 
{

  digitalWrite(SDCARDLED, HIGH);
  //Making string to push to file
  String Push = "";
  DateTime now = rtc.now(); //Get current time and date

  String Date = String(now.month()) + "/" + String(now.day()) + "/" + String(now.year()); //Create the Date string
  String Time = String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second()); //Create the Time string

  float Humidity = dht.readHumidity(); //Read the humidity
  float Temp = dht.readTemperature(); //Read the temperature
  
  Push = Date + "," + Time + ","; //Create beginning of string
  
  File dataFile = SD.open(FILENAME, FILE_WRITE); //Open file on SD card
  
  dataFile.print(Push); //Push the tempeature
  dataFile.print(Temp); //Push the seperator
  dataFile.print(","); //Push the seperator
  dataFile.print(Humidity); //Push the humidity
  dataFile.print("\n"); //Push line deliminator
  dataFile.close(); //Close the file

  digitalWrite(SDCARDLED, LOW);
  delay(60000); //Wait 1 minute

}
