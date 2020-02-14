/*
  Output the temperature readings to all pixels to be read by a Processing visualizer    
  By: Nathan Seidle
  SparkFun Electronics
  Date: May 22nd, 2018
  License: MIT. See license file for more information but you can
  basically do whatever you want with this code.
  Feel like supporting open source hardware?
  Buy a board from SparkFun! https://www.sparkfun.com/products/14769
  This example outputs 768 temperature values as fast as possible. Use this example
  in conjunction with our Processing visualizer.
  This example will work with a Teensy 3.1 and above. The MLX90640 requires some
  hefty calculations and larger arrays. You will need a microcontroller with 20,000
  bytes or more of RAM.
  This relies on the driver written by Melexis and can be found at:
  https://github.com/melexis/mlx90640-library
  Hardware Connections:
  Connect the SparkFun Qwiic Breadboard Jumper (https://www.sparkfun.com/products/14425)
  to the Qwiic board
  Connect the male pins to the Teensy. The pinouts can be found here: https://www.pjrc.com/teensy/pinout.html
  Open the serial monitor at 115200 baud to see the output
*/

#include <Wire.h>
#include <SPI.h>
#include "SdFat.h"
SdFatSdioEX SD;

#include "MLX90640_API.h"
#include "MLX90640_I2C_Driver.h"
#include <SparkFun_RV1805.h>

// initializing RTC

RV1805 rtc;
String currentTime;

//The below variables control what the date will be set to
//int hund = 50;
//int sec = 2;
//int minute = 18;
//int hour = 7;
//int date = 23;
//int month = 1;
//int year = 2020;
//int day = 5;

// initializing MLX
const byte MLX90640_address = 0x33; //Default 7-bit unshifted address of the MLX90640

#define TA_SHIFT 8 //Default shift for MLX90640 in open air

float mlx90640To[768];
paramsMLX90640 mlx90640;

// lEd pin
const int ledPin = 13;
//const int chipSelect = BUILTIN_SDCARD;
File myFile;

String data;


void setup()
{
  Wire.begin();
  Wire.setClock(400000); //Increase I2C clock speed to 400kHz
 //Wire.setClock(4000000);

  Serial.begin(115200); //Fast serial as possible

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  
//  while (!Serial); //Wait for user to open terminal
  
  if (!SD.begin()) {
    Serial.println("initialization failed!");
    digitalWrite(ledPin, LOW);
    return;
  }

  
  
  if (rtc.begin() == false)
   {
    Serial.println("Something went wrong, check wiring");
    digitalWrite(ledPin, LOW);
   }
  
  //Serial.println("RTC connected!");

  if (rtc.setToCompilerTime() == false) { //
    Serial.println("Something went wrong setting the time");
  }

     rtc.setHundredths(0);//
  
  
  if (isConnected() == false)
  {
    Serial.println("MLX90640 not detected at default I2C address. Please check wiring. Freezing.");
    while (1);
  }

  //Get device parameters - We only have to do this once
  int status;
  uint16_t eeMLX90640[832];
  status = MLX90640_DumpEE(MLX90640_address, eeMLX90640);
  if (status != 0)
    Serial.println("Failed to load system parameters");

  status = MLX90640_ExtractParameters(eeMLX90640, &mlx90640);
  if (status != 0)
    Serial.println("Parameter extraction failed");

  //Once params are extracted, we can release eeMLX90640 array

  //MLX90640_SetRefreshRate(MLX90640_address, 0x02); //Set rate to 2Hz
   //MLX90640_SetRefreshRate(MLX90640_address, 0x03); //Set rate to 4Hz
  MLX90640_SetRefreshRate(MLX90640_address, 0x04); //Set rate to 64Hz

  //MLX90640_SetResolution(MLX90640_address,0x03);                                                           

//   Serial.println(MLX90640_GetCurResolution (MLX90640_address));
}

void loop()
{
  digitalWrite(ledPin, LOW);
  


  if(rtc.updateTime() == false)
  {
    Serial.print("RTC failed to update");
    digitalWrite(ledPin, LOW);
   }

   else
   {
     digitalWrite(ledPin, HIGH);
    }

//    if (rtc.setTime(hund, sec, minute, hour, date, month, year, day) == false) {
//    Serial.println("Something went wrong setting the time");
//    }

   // getting time 
   currentTime = rtc.stringTimeStamp(); 

  // Creating a file 
  myFile = SD.open("MLX.csv", FILE_WRITE);

  //Serial.println(currentTime);
  
  long startTime = millis();  
  data = currentTime+ ","+startTime+","; // missed the comma here after startTime
  //data = ",";
  for (byte x = 0 ; x < 2 ; x++)
  {
    uint16_t mlx90640Frame[834];
    int status = MLX90640_GetFrameData(MLX90640_address, mlx90640Frame);

    float vdd = MLX90640_GetVdd(mlx90640Frame, &mlx90640);
    float Ta = MLX90640_GetTa(mlx90640Frame, &mlx90640);

    float tr = Ta - TA_SHIFT; //Reflected temperature based on the sensor ambient temperature
    float emissivity = 0.95;

    MLX90640_CalculateTo(mlx90640Frame, &mlx90640, emissivity, tr, mlx90640To);
  }
  long stopTime = millis();

  for (int x = 0 ; x < 768 ; x++)
  {
    //if(x % 8 == 0) Serial.println();
    data = data + mlx90640To[x] + ",";
//    Serial.print(mlx90640To[x], 2);
//    Serial.print(" ");
  }
  Serial.println(data);

  if(myFile==1)
  {
    //myFile.println(currentTime+","+millis()+","+data); // perhaps add currentTime
    myFile.println(data);
    digitalWrite(ledPin, HIGH);

    myFile.close();
//  delay(50);
  }

  //myFile.close();
  //delay(50);

  else
  {
    Serial.print("error");
    digitalWrite(ledPin, LOW);
    
    }

}

//Returns true if the MLX90640 is detected on the I2C bus
boolean isConnected()
{
  Wire.beginTransmission((uint8_t)MLX90640_address);
   //Wire.beginTransmission((uint16_t)MLX90640_address);
  if (Wire.endTransmission() != 0)
    return (false); //Sensor did not ACK
  return (true);
}
