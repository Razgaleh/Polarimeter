#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_FT6206.h>
#include <Wire.h>
#include <Fonts/FreeSans12pt7b.h>

#define TFT_DC 9
#define TFT_CS 10
#define BOXSIZE 40

Adafruit_FT6206 ctp = Adafruit_FT6206();
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

float pcontrast = 33;
float dbcontrast = 23;

float pexratio = 40;
float dbexratio = 23;

float pdop = 30;
float dbdop = 24;

void setup() {


  Serial.begin(115200);
  Serial.println("TFT Test!");
  tft.begin();
  ctp.begin();

  tft.setRotation(1);                            //initializing
  tft.fillScreen(ILI9341_BLACK);
  tft.setFont(&FreeSans12pt7b);

  tft.setCursor(140, 23);                        // Data Box
  tft.setTextSize(1);
  tft.setTextColor(ILI9341_RED);
  tft.fillRect(0, 0, 320, 30, ILI9341_WHITE);
  tft.println("Data"); 
/*------------------------------------*/
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(33, 83);
  tft.drawRect(30, 60, 110, 30, ILI9341_WHITE);       //contrast
  tft.println(" Contrast");


  tft.setCursor(161, 83);                             // print percent contrast
  tft.drawRect(160, 60, 60, 30, ILI9341_WHITE);
  tft.println(pcontrast);

  tft.setCursor(243, 83);                              // selecting the % box red contrast
  tft.drawRect(240, 60, 30, 30, ILI9341_BLACK);
  tft.drawRect(240, 60, 30, 30, ILI9341_RED);
  tft.setTextColor(ILI9341_RED);
  tft.println("%");

  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(273, 83);
  tft.drawRect(270, 60, 35, 30, ILI9341_WHITE);       //dB box
  tft.println("dB");
  /* -------------------------------*/
  tft.setCursor(35, 143);
  tft.drawRect(30, 120, 110, 30, ILI9341_WHITE);   //Extinction Ratio
  tft.setTextSize(1);
  tft.println("Extinction ");

  tft.setTextSize(1);                             //print percent Extrinction Ratio
  tft.setCursor(161, 143);
  tft.drawRect(160, 120, 60, 30, ILI9341_WHITE);
  tft.println(dbexratio);

  tft.setCursor(273, 143);                       // selecting dB box red
  tft.fillRect(270, 120, 35, 30, ILI9341_BLACK);
  tft.drawRect(270, 120, 35, 30, ILI9341_RED);
  tft.setTextColor(ILI9341_RED);
  tft.println("dB");

  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(243, 143);                        //  % box
  tft.drawRect(240, 120, 30, 30, ILI9341_WHITE);
  tft.println("%");
  /* -------------------------------*/
  tft.setCursor(37, 203);
  tft.drawRect(30, 180, 110, 30, ILI9341_WHITE);       // Degree of circular polarization
  tft.setTextSize(1);
  tft.println("    DOP");

  tft.setTextSize(1);                                 // print percent DOP
  tft.setCursor(161, 203);
  tft.drawRect(160, 180, 60, 30, ILI9341_WHITE);
  tft.println(pdop);

  tft.setCursor(243, 203);                            // selecting the % box red DOP
  tft.fillRect(240, 180, 30, 30, ILI9341_BLACK);
  tft.drawRect(240, 180, 30, 30, ILI9341_RED);
  tft.setTextColor(ILI9341_RED);
  tft.println("%");

  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(273, 203);                             // dB box
  tft.drawRect(270, 180, 35, 30, ILI9341_WHITE);
  tft.println("dB");

}


void loop() {


  if (! ctp.touched()) {      // checking if screen is touched
    return;
  }

  TS_Point p = ctp.getPoint();

  Serial.print("X = "); Serial.print(p.x);        // Printing our coordinates on TFT Screen
  Serial.print("\tY = "); Serial.print(p.y);
  Serial.print("->");
  Serial.print("("); Serial.print(p.x);
  Serial.print(", "); Serial.print(p.y);
  Serial.println(")");



  { // commented data tab (refresh button)
    /* if  (p.x >= 0 && p.x <= 25 && p.y >= 0 && p.y <= 320) { //data tab -> Refresh Button


       tft.fillRect(0, 60, 320, 30, ILI9341_BLACK);
       tft.fillRect(0, 120, 320, 30, ILI9341_BLACK);
       tft.fillRect(0, 180, 320, 30, ILI9341_BLACK);


       tft.setCursor(33, 83);       // Contrast
       tft.drawRect(30, 60, 110, 30, ILI9341_WHITE);
       tft.println("Contrast");


       tft.setCursor(163, 83);
       tft.drawRect(160, 60, 60, 30, ILI9341_WHITE);
       tft.println(pcontrast);

       tft.setCursor(243, 83);
       tft.drawRect(240, 60, 30, 30, ILI9341_WHITE);
       tft.println("%");

       tft.setCursor(273, 83);
       tft.drawRect(270, 60, 35, 30, ILI9341_WHITE);
       tft.println("dB"); */

    /* -------------------------------*/

    /*tft.setCursor(33, 143);
      tft.drawRect(30, 120, 110, 30, ILI9341_WHITE);   //Extinction Ratio
      tft.setTextSize(1);
      tft.println("Extinction ");

      tft.setTextSize(1);
      tft.setCursor(163, 143);
      tft.drawRect(160, 120, 60, 30, ILI9341_WHITE);
      tft.println(pexratio);

      tft.setCursor(243, 143);
      tft.drawRect(240, 120, 30, 30, ILI9341_WHITE);
      tft.println("%");

      tft.setCursor(273, 143);
      tft.drawRect(270, 120, 35, 30, ILI9341_WHITE);
      tft.println("dB"); */

    /* -------------------------------*/

    /* tft.setCursor(33, 203);
      tft.drawRect(30, 180, 110, 30, ILI9341_WHITE); //Degree of circular polarization
      tft.setTextSize(1);
      tft.println("    DOP");

      tft.setTextSize(1);
      tft.setCursor(163, 203);
      tft.drawRect(160, 180, 60, 30, ILI9341_WHITE);
      tft.println(pdop);

      tft.setCursor(243, 203);
      tft.drawRect(240, 180, 30, 30, ILI9341_WHITE);
      tft.println("%");

      tft.setCursor(273, 203);
      tft.drawRect(270, 180, 35, 30, ILI9341_WHITE);
      tft.println("dB");

      } */
  }



  if (p.x >= 60 && p.x <= 90 && p.y >= 49 && p.y <= 80) {  // contrast percentage

    tft.setCursor(273, 83);                       // Reseting dB box to white
    tft.fillRect(270, 60, 35, 30, ILI9341_BLACK);
    tft.drawRect(270, 60, 35, 30, ILI9341_WHITE);
    tft.setTextColor(ILI9341_WHITE);
    tft.println("dB");


    tft.setCursor(243, 83);                    // selecting the % box red
    tft.fillRect(240, 60, 30, 30, ILI9341_BLACK);
    tft.drawRect(240, 60, 30, 30, ILI9341_RED);
    tft.setTextColor(ILI9341_RED);
    tft.println("%");

    tft.setTextColor(ILI9341_WHITE);             // printing the number
    tft.setCursor(161, 83);
    tft.fillRect(160, 60, 60, 30, ILI9341_BLACK);
    tft.drawRect(160, 60, 60, 30, ILI9341_WHITE);
    tft.println(pcontrast);

  }
  else if (p.x >= 60 && p.x <= 90 && p.y >= 15 && p.y <= 50) {   // contrast db
    tft.setCursor(243, 83);                    // resetting the % box to white
    tft.fillRect(240, 60, 30, 30, ILI9341_BLACK);
    tft.drawRect(240, 60, 30, 30, ILI9341_WHITE);
    tft.setTextColor(ILI9341_WHITE);
    tft.println("%");

    tft.setCursor(273, 83);                       // selecting dB box red
    tft.fillRect(270, 60, 35, 30, ILI9341_BLACK);
    tft.drawRect(270, 60, 35, 30, ILI9341_RED);
    tft.setTextColor(ILI9341_RED);
    tft.println("dB");


    tft.setTextColor(ILI9341_WHITE);
    tft.setCursor(161, 83);
    tft.fillRect(160, 60, 60, 30, ILI9341_BLACK);
    tft.drawRect(160, 60, 60, 30, ILI9341_WHITE);
    tft.println(dbcontrast);



  }
  /* -------------------------------*/
  else if (p.x >= 120 && p.x <= 150 && p.y >= 49 && p.y <= 80) {  // Extinction Ratio percentage
    tft.setCursor(273, 143);                       // Reseting dB box to white
    tft.fillRect(270, 120, 35, 30, ILI9341_BLACK);
    tft.drawRect(270, 120, 35, 30, ILI9341_WHITE);
    tft.setTextColor(ILI9341_WHITE);
    tft.println("dB");


    tft.setCursor(243, 143);                    // selecting the % box red
    tft.drawRect(240, 120, 30, 30, ILI9341_BLACK);
    tft.drawRect(240, 120, 30, 30, ILI9341_RED);
    tft.setTextColor(ILI9341_RED);
    tft.println("%");

    tft.setTextColor(ILI9341_WHITE);             // printing the number
    tft.setCursor(161, 143);
    tft.fillRect(160, 120, 60, 30, ILI9341_BLACK);
    tft.drawRect(160, 120, 60, 30, ILI9341_WHITE);
    tft.println(pexratio);

  }

  else if (p.x >= 120 && p.x <= 150 && p.y >= 15 && p.y <= 50) {   // Extinction  Ratio db
    tft.setCursor(243, 143);                    // resetting the % box to white
    tft.fillRect(240, 120, 30, 30, ILI9341_BLACK);
    tft.drawRect(240, 120, 30, 30, ILI9341_WHITE);
    tft.setTextColor(ILI9341_WHITE);
    tft.println("%");

    tft.setCursor(273, 143);                       // selecting dB box red
    tft.fillRect(270, 120, 35, 30, ILI9341_BLACK);
    tft.drawRect(270, 120, 35, 30, ILI9341_RED);
    tft.setTextColor(ILI9341_RED);
    tft.println("dB");


    tft.setTextColor(ILI9341_WHITE);
    tft.setCursor(161, 143);
    tft.fillRect(160, 120, 60, 30, ILI9341_BLACK);
    tft.drawRect(160, 120, 60, 30, ILI9341_WHITE);
    tft.println(dbexratio);

  }
  /* -------------------------------*/
  else if (p.x >= 180 && p.x <= 210 && p.y >= 49 && p.y <= 80) {  // DOP percentage
    tft.setCursor(273, 203);                       // Reseting dB box to white
    tft.fillRect(270, 180, 35, 30, ILI9341_BLACK);
    tft.drawRect(270, 180, 35, 30, ILI9341_WHITE);
    tft.setTextColor(ILI9341_WHITE);
    tft.println("dB");


    tft.setCursor(243, 203);                    // selecting the % box red
    tft.fillRect(240, 180, 30, 30, ILI9341_BLACK);
    tft.drawRect(240, 180, 30, 30, ILI9341_RED);
    tft.setTextColor(ILI9341_RED);
    tft.println("%");

    tft.setTextColor(ILI9341_WHITE);             // printing the number
    tft.setCursor(161, 203);
    tft.fillRect(160, 180, 60, 30, ILI9341_BLACK);
    tft.drawRect(160, 180, 60, 30, ILI9341_WHITE);
    tft.println(pdop);
  }

  else if (p.x >= 180 && p.x <= 210 && p.y >= 15 && p.y <= 50) {   // DOP db
    tft.setCursor(243, 203);                    // resetting the % box to white
    tft.fillRect(240, 180, 30, 30, ILI9341_BLACK);
    tft.drawRect(240, 180, 30, 30, ILI9341_WHITE);
    tft.setTextColor(ILI9341_WHITE);
    tft.println("%");

    tft.setCursor(273, 203);                       // selecting dB box red
    tft.fillRect(270, 180, 35, 30, ILI9341_BLACK);
    tft.drawRect(270, 180, 35, 30, ILI9341_RED);
    tft.setTextColor(ILI9341_RED);
    tft.println("dB");


    tft.setTextColor(ILI9341_WHITE);
    tft.setCursor(161, 203);
    tft.fillRect(160, 180, 60, 30, ILI9341_BLACK);
    tft.drawRect(160, 180, 60, 30, ILI9341_WHITE);
    tft.println(dbdop);
  }
/*---------------------------------*/
  else {
    return;
  }

}

