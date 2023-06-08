#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_FT6206.h>
#include <Wire.h>
#include <Fonts/FreeSans12pt7b.h>
#include <math.h>

#define TFT_DC 9                                 // pins for touch screen
#define TFT_CS 10
#define SIZE  (sizeof(data)/sizeof(float))       // constant; Showing the length of a float array 


Adafruit_FT6206 ctp = Adafruit_FT6206();
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);


float maxvoltage = 4.5;                 
float R1 = 75000;                               // 1st Resistor
float R2 = 51000;                               // 2nd Resistor

int c=0,d=0,e=0;                                // c=Contrast - d=DOP - e=Extinction Ratio // default cases  


void setup() {


  Serial.begin(115200);
  Serial.println("TFT Test!");
  tft.begin();
  ctp.begin();

  tft.setRotation(1);                            //initializing
  tft.fillScreen(ILI9341_BLACK);
  tft.setFont(&FreeSans12pt7b);


  float pcontrast = 0;
  float dbcontrast = 0;

  float pexratio = 0;
  float dbexratio = 0;

  float pdop = 0;
  float dbdop = 0;

  tft.setCursor(70, 23);                        // Data Box
  tft.setTextSize(1);
  tft.setTextColor(ILI9341_RED);
  tft.fillRect(0, 0, 320, 30, ILI9341_WHITE);
  tft.println("Polarization Specs");


  /*--------------------------------------------------------------------------------------------------------------------*/

// Displaying "Contrast" and "%" + "dB" Boxes

  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(15, 83);
  //  tft.drawRect(15, 60, 110, 30, ILI9341_WHITE);       //contrast
  tft.println("Contrast");


  tft.setCursor(141, 83);                             // print percent contrast
  //  tft.drawRect(140, 60, 60, 30, ILI9341_WHITE);
  tft.println(pcontrast);

  tft.setCursor(233, 83);                              // selecting the % box red contrast
  tft.drawRect(230, 60, 30, 30, ILI9341_BLACK);
  tft.drawRect(230, 60, 30, 30, ILI9341_RED);
  tft.setTextColor(ILI9341_RED);
  tft.println("%");

  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(263, 83);
  tft.drawRect(260, 60, 35, 30, ILI9341_WHITE);      //dB box
  tft.println("dB");


  /* ------------------------------------------------------------------------------------------------------------------*/


// Displaying "Extinction Ratio" and "%" + "dB" Boxes

  tft.setCursor(15, 143);
  //  tft.drawRect(15, 120, 110, 30, ILI9341_WHITE);     //Extinction Ratio
  tft.setTextSize(1);
  tft.println("Extinction");

  tft.setTextSize(1);                               //print percent Extrinction Ratio
  tft.setCursor(141, 143);
  //  tft.drawRect(140, 120, 60, 30, ILI9341_WHITE);
  tft.println(dbexratio);

  tft.setCursor(263, 143);                          // selecting dB box red
  tft.fillRect(260, 120, 35, 30, ILI9341_BLACK);
  tft.drawRect(260, 120, 35, 30, ILI9341_RED);
  tft.setTextColor(ILI9341_RED);
  tft.println("dB");

  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(233, 143);                            // % box
  tft.drawRect(230, 120, 30, 30, ILI9341_WHITE);
  tft.println("%");


  /* -------------------------------------------------------------------------------------------------------------------*/

// Displaying "DOP" and "%" + "dB" Boxes

  tft.setCursor(15, 203);
  //  tft.drawRect(15, 180, 110, 30, ILI9341_WHITE);       // Degree of circular polarization
  tft.setTextSize(1);
  tft.println("DOP");

  tft.setTextSize(1);                                 // print percent DOP
  tft.setCursor(141, 203);
  //  tft.drawRect(140, 180, 60, 30, ILI9341_WHITE);
  tft.println(pdop);

  tft.setCursor(233, 203);                            // selecting the % box red DOP
  tft.fillRect(230, 180, 30, 30, ILI9341_BLACK);
  tft.drawRect(230, 180, 30, 30, ILI9341_RED);
  tft.setTextColor(ILI9341_RED);
  tft.println("%");

  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(263, 203);                             // dB box
  tft.drawRect(260, 180, 35, 30, ILI9341_WHITE);
  tft.println("dB");
}

void loop() {


  int looop = 1;
  float data[10] = {};                                  //taking 10 data each loop-fast enough 
  
  getdata(data, SIZE);                                 // Getting DATA

  float MAX = getmax(data, SIZE);
  float MIN = getmin(data, SIZE);

  float contrast = (MAX - MIN) / (MAX + MIN);
  float pcontrast = 100 * contrast;
  float dbcontrast = (-10) * log10(contrast);

  float pexratio = 100 * (MIN / MAX);
  float dbexratio = (-10) * log10(MIN / MAX);


  float pdop = 100 - pcontrast;                       
  float dbdop = (-10) * log10(1 - (contrast));



  /*                                                DEFAULT SELECTIONS  (  Contrast in % ; Extinction Ratio in dB ; DOP in %  )                                    */
  while (c == 0 && d == 0 && e == 0) {

    tft.setTextColor(ILI9341_WHITE);                   // printing the % CONTRAST
    tft.setCursor(141, 83);
    tft.fillRect(140, 60, 71, 30, ILI9341_BLACK);
    //    tft.drawRect(140, 60, 60, 30, ILI9341_WHITE);
    tft.println(pcontrast, 2);

    delay(30);

    tft.setTextColor(ILI9341_WHITE);                  //printing dB EXTIONTION RATIO
    tft.setCursor(141, 143);
    tft.fillRect(140, 120, 71, 30, ILI9341_BLACK);
    //    tft.drawRect(140, 120, 60, 30, ILI9341_WHITE);
    tft.println(dbexratio, 2);

    delay(35);

    tft.setTextColor(ILI9341_WHITE);                   // printing the % DOP
    tft.setCursor(141, 203);
    tft.fillRect(140, 180, 71, 30, ILI9341_BLACK);
    //    tft.drawRect(140, 180, 60, 30, ILI9341_WHITE);
    tft.println(pdop, 2);

    delay(25);

    break;
  }


                                             /* Getting points */

  if (! ctp.touched()) {                              // checking if screen is being touched
    return;
  }

  
  TS_Point  p = ctp.getPoint();


  Serial.print("X = "); Serial.print(p.x);             // Printing our coordinates on TFT screen
  Serial.print("\tY = "); Serial.print(p.y);
  Serial.print("->");
  Serial.print("("); Serial.print(p.x);
  Serial.print(", "); Serial.print(p.y);
  Serial.println(")");


  /*----------------------------------------------------------------------------------------------------------------------*/


  while (p.x >= 30 && p.x <= 105 && p.y >= 61 && p.y <= 200) {       /* Contrast percentage  */

    getdata(data, SIZE);                                          // Getting DATA
    boolean endpt = false;

    float MAX = getmax(data, SIZE);
    float MIN = getmin(data, SIZE);

    float contrast = (MAX - MIN) / (MAX + MIN);
    float pcontrast = 100 * contrast;

    float pexratio = 100 * (MIN / MAX);
    float dbexratio = (-10) * log10(MIN / MAX);

    float pdop = 100 - pcontrast;               //degree of circular polarization
    float dbdop = (-10) * log10(1 - contrast);


    while (looop == 1) {                             //graphics --- runs the loop only one time 

      tft.setCursor(263, 83);                       // Reseting dB box to white
      tft.fillRect(260, 60, 35, 30, ILI9341_BLACK);
      tft.drawRect(260, 60, 35, 30, ILI9341_WHITE);
      tft.setTextColor(ILI9341_WHITE);
      tft.println("dB");


      tft.setCursor(233, 83);                    // selecting the % box red
      tft.fillRect(230, 60, 30, 30, ILI9341_BLACK);
      tft.drawRect(230, 60, 30, 30, ILI9341_RED);
      tft.setTextColor(ILI9341_RED);
      tft.println("%");
      looop = looop + 1;
    }


    tft.setTextColor(ILI9341_WHITE);            // printing  contrast %
    tft.setCursor(141, 83);
    tft.fillRect(140, 60, 71, 30, ILI9341_BLACK);
    //    tft.drawRect(140, 60, 60, 30, ILI9341_WHITE);
    tft.println(pcontrast, 2);

/*                                       Setting the setup as it was before                              */
    while (e == 2 || e == 0) {           // Extinction Ratio dB
      tft.setTextColor(ILI9341_WHITE);     // db exratio
      tft.setCursor(141, 143);
      tft.fillRect(140, 120, 71, 30, ILI9341_BLACK);
      //    tft.drawRect(140, 120, 60, 30, ILI9341_WHITE);
      tft.println(dbexratio, 2);
      break;
    }


    while (e == 1) {                       // Extinction Ratio %
      tft.setTextColor(ILI9341_WHITE);             // printing the number
      tft.setCursor(141, 143);
      tft.fillRect(140, 120, 71, 30, ILI9341_BLACK);
      //    tft.drawRect(140, 120, 60, 30, ILI9341_WHITE);
      tft.println(pexratio, 2);
      break;
    }

    while (d == 1 || d == 0) {             // DOP %
      tft.setTextColor(ILI9341_WHITE);             // printing the number
      tft.setCursor(141, 203);
      tft.fillRect(140, 180, 71, 30, ILI9341_BLACK);
      //    tft.drawRect(140, 180, 60, 30, ILI9341_WHITE);
      tft.println(pdop);
      break;
    }

    while ( d == 2) {                     // DOP dB

      tft.setTextColor(ILI9341_WHITE);
      tft.setCursor(141, 203);
      tft.fillRect(140, 180, 71, 30, ILI9341_BLACK);
      // tft.drawRect(140, 180, 60, 30, ILI9341_WHITE);
      tft.println(dbdop, 2);
      break;
    }

                                          /* Getting Points */

    TS_Point p = ctp.getPoint();
    Serial.print("("); Serial.print(p.x);
    Serial.print(", "); Serial.print(p.y);
    Serial.println(")");
    
    while (p.x >= 105 && p.x <= 165 && p.y >= 61 && p.y <= 200) { // Extinction Ratio %


      tft.setCursor(263, 143);                       // Reseting dB box to white
      tft.fillRect(260, 120, 35, 30, ILI9341_BLACK);
      tft.drawRect(260, 120, 35, 30, ILI9341_WHITE);
      tft.setTextColor(ILI9341_WHITE);
      tft.println("dB");


      tft.setCursor(233, 143);                    // selecting the % box red
      tft.drawRect(230, 120, 30, 30, ILI9341_BLACK);
      tft.drawRect(230, 120, 30, 30, ILI9341_RED);
      tft.setTextColor(ILI9341_RED);
      tft.println("%");
      e = 1;
      endpt = true;
      break;
    }

    while (p.x >= 105 && p.x <= 165 && p.y >= 1 && p.y <= 60) {   // Extinction  Ratio db
      tft.setCursor(233, 143);                    // resetting the % box to white
      tft.fillRect(230, 120, 30, 30, ILI9341_BLACK);
      tft.drawRect(230, 120, 30, 30, ILI9341_WHITE);
      tft.setTextColor(ILI9341_WHITE);
      tft.println("%");

      tft.setCursor(263, 143);                       // selecting dB box red
      tft.fillRect(260, 120, 35, 30, ILI9341_BLACK);
      tft.drawRect(260, 120, 35, 30, ILI9341_RED);
      tft.setTextColor(ILI9341_RED);
      tft.println("dB");
      e = 2;
      endpt = true;
      break;
    }
  
    while (p.x > 165 && p.x <= 240 && p.y >= 61 && p.y <= 200) {  // DOP %
      endpt = true;
      tft.setCursor(263, 203);                       // Reseting dB box to white
      tft.fillRect(260, 180, 35, 30, ILI9341_BLACK);
      tft.drawRect(260, 180, 35, 30, ILI9341_WHITE);
      tft.setTextColor(ILI9341_WHITE);
      tft.println("dB");


      tft.setCursor(233, 203);                    // selecting the % box red
      tft.fillRect(230, 180, 30, 30, ILI9341_BLACK);
      tft.drawRect(230, 180, 30, 30, ILI9341_RED);
      tft.setTextColor(ILI9341_RED);
      tft.println("%");
      break;
    }
    
    while (p.x > 165 && p.x <= 240 && p.y >= 1  && p.y <= 60) { // DOP dB

      endpt = true;
      tft.setCursor(233, 203);                    // resetting the % box to white
      tft.fillRect(230, 180, 30, 30, ILI9341_BLACK);
      tft.drawRect(230, 180, 30, 30, ILI9341_WHITE);
      tft.setTextColor(ILI9341_WHITE);
      tft.println("%");

      tft.setCursor(263, 203);                       // selecting dB box red
      tft.fillRect(260, 180, 35, 30, ILI9341_BLACK);
      tft.drawRect(260, 180, 35, 30, ILI9341_RED);
      tft.setTextColor(ILI9341_RED);
      tft.println("dB");
      break;
    }
   

    while (p.x >= 30 && p.x <= 105 && p.y >= 1 && p.y <= 60) {  // Contrast dB
      endpt = true;
      tft.setCursor(233, 83);                    // resetting the % box to white
      tft.fillRect(230, 60, 30, 30, ILI9341_BLACK);
      tft.drawRect(230, 60, 30, 30, ILI9341_WHITE);
      tft.setTextColor(ILI9341_WHITE);
      tft.println("%");

      tft.setCursor(263, 83);                       // selecting dB box red
      tft.fillRect(260, 60, 35, 30, ILI9341_BLACK);
      tft.drawRect(260, 60, 35, 30, ILI9341_RED);
      tft.setTextColor(ILI9341_RED);
      tft.println("dB");
      break;
    }

    if (endpt == true) {    // ends the loop if contrast dB is pressed 
      c = 1;
      break;
    }
  
    c = 1;
    delay(100);
  } 

  while  (p.x >= 30 && p.x <= 105 && p.y >= 1 && p.y <= 60) {    /* Contrast db */

   
    getdata(data, SIZE);                // Getting DATA
    boolean endpt = false;
    boolean zero = false;
    float MAX = getmax(data, SIZE);
    float MIN = getmin(data, SIZE);

    float contrast = (MAX - MIN) / (MAX + MIN);
    float dbcontrast = (-10) * log10(contrast);

    float pexratio = 100 * (MIN / MAX);
    float dbexratio = (-10) * log10(MIN / MAX);

    float pdop = 100 - pcontrast;               //degree of circular polarization
    float dbdop = (-10) * log10(1 - (contrast));
    

    while (looop == 1) {                    // graphics --- runs this loop one time 
      tft.setCursor(233, 83);                    // resetting the % box to white
      tft.fillRect(230, 60, 30, 30, ILI9341_BLACK);
      tft.drawRect(230, 60, 30, 30, ILI9341_WHITE);
      tft.setTextColor(ILI9341_WHITE);
      tft.println("%");

      tft.setCursor(263, 83);                       // selecting dB box red
      tft.fillRect(260, 60, 35, 30, ILI9341_BLACK);
      tft.drawRect(260, 60, 35, 30, ILI9341_RED);
      tft.setTextColor(ILI9341_RED);
      tft.println("dB");
      looop = looop + 1;
    }

    tft.setTextColor(ILI9341_WHITE);                      // printing contrast in dB
    tft.setCursor(141, 83);
    tft.fillRect(140, 60, 71, 30, ILI9341_BLACK);
    //    tft.drawRect(140, 60, 60, 30, ILI9341_WHITE);
    tft.println(dbcontrast, 2);


    while (e == 2 || e == 0) {       // Extinction Ratio dB
      tft.setTextColor(ILI9341_WHITE);     // db exratio
      tft.setCursor(141, 143);
      tft.fillRect(140, 120, 71, 30, ILI9341_BLACK);
      //    tft.drawRect(140, 120, 60, 30, ILI9341_WHITE);
      tft.println(dbexratio, 2);
      break;
    }

    while (e == 1) {                //  Extinction Ratio %
      tft.setTextColor(ILI9341_WHITE);             // printing the number
      tft.setCursor(141, 143);
      tft.fillRect(140, 120, 71, 30, ILI9341_BLACK);
      //    tft.drawRect(140, 120, 60, 30, ILI9341_WHITE);
      tft.println(pexratio, 2);
      break;
    }


    while (d == 1 || d == 0 ) {    // DOP %
      tft.setTextColor(ILI9341_WHITE);             // printing the number
      tft.setCursor(141, 203);
      tft.fillRect(140, 180, 71, 30, ILI9341_BLACK);
      //    tft.drawRect(140, 180, 60, 30, ILI9341_WHITE);
      tft.println(pdop);
      break;
    }

    while ( d == 2) {            // DOP dB

      tft.setTextColor(ILI9341_WHITE);
      tft.setCursor(141, 203);
      tft.fillRect(140, 180, 71, 30, ILI9341_BLACK);
      // tft.drawRect(140, 180, 60, 30, ILI9341_WHITE);
      tft.println(dbdop, 2);
      break;
    }

   
    /* Getting points */
    
    TS_Point p = ctp.getPoint();
    Serial.print("("); Serial.print(p.x);
    Serial.print(", "); Serial.print(p.y);
    Serial.println(")");


 

    while (p.x >= 105 && p.x <= 165 && p.y >= 61 && p.y <= 200) { // Extinction Ratio %


      tft.setCursor(263, 143);                       // Reseting dB box to white
      tft.fillRect(260, 120, 35, 30, ILI9341_BLACK);
      tft.drawRect(260, 120, 35, 30, ILI9341_WHITE);
      tft.setTextColor(ILI9341_WHITE);
      tft.println("dB");


      tft.setCursor(233, 143);                    // selecting the % box red
      tft.drawRect(230, 120, 30, 30, ILI9341_BLACK);
      tft.drawRect(230, 120, 30, 30, ILI9341_RED);
      tft.setTextColor(ILI9341_RED);
      tft.println("%");
      e = 1;
      endpt = true;
      break;
    }

    while (p.x >= 105 && p.x <= 165 && p.y >= 1 && p.y <= 60) {   // Extinction  Ratio db
      tft.setCursor(233, 143);                    // resetting the % box to white
      tft.fillRect(230, 120, 30, 30, ILI9341_BLACK);
      tft.drawRect(230, 120, 30, 30, ILI9341_WHITE);
      tft.setTextColor(ILI9341_WHITE);
      tft.println("%");

      tft.setCursor(263, 143);                       // selecting dB box red
      tft.fillRect(260, 120, 35, 30, ILI9341_BLACK);
      tft.drawRect(260, 120, 35, 30, ILI9341_RED);
      tft.setTextColor(ILI9341_RED);
      tft.println("dB");
      e = 2;
      endpt = true;
      break;
    }
   
    while (p.x > 165 && p.x <= 240 && p.y >= 61 && p.y <= 200) {  // DOP %
      endpt = true;
      tft.setCursor(263, 203);                       // Reseting dB box to white
      tft.fillRect(260, 180, 35, 30, ILI9341_BLACK);
      tft.drawRect(260, 180, 35, 30, ILI9341_WHITE);
      tft.setTextColor(ILI9341_WHITE);
      tft.println("dB");


      tft.setCursor(233, 203);                    // selecting the % box red
      tft.fillRect(230, 180, 30, 30, ILI9341_BLACK);
      tft.drawRect(230, 180, 30, 30, ILI9341_RED);
      tft.setTextColor(ILI9341_RED);
      tft.println("%");
      break;
    }
   
    while (p.x > 165 && p.x <= 240 && p.y >= 1  && p.y <= 60) { // DOP dB

      endpt = true;
      tft.setCursor(233, 203);                    // resetting the % box to white
      tft.fillRect(230, 180, 30, 30, ILI9341_BLACK);
      tft.drawRect(230, 180, 30, 30, ILI9341_WHITE);
      tft.setTextColor(ILI9341_WHITE);
      tft.println("%");

      tft.setCursor(263, 203);                       // selecting dB box red
      tft.fillRect(260, 180, 35, 30, ILI9341_BLACK);
      tft.drawRect(260, 180, 35, 30, ILI9341_RED);
      tft.setTextColor(ILI9341_RED);
      tft.println("dB");
      break;
    }
    


    while (p.x >= 30 && p.x <= 105 && p.y >= 61 && p.y <= 200) {  // Contrast dB
      endpt = true;
      tft.setCursor(263, 83);                       // Reseting dB box to white
      tft.fillRect(260, 60, 35, 30, ILI9341_BLACK);
      tft.drawRect(260, 60, 35, 30, ILI9341_WHITE);
      tft.setTextColor(ILI9341_WHITE);
      tft.println("dB");


      tft.setCursor(233, 83);                    // selecting the % box red
      tft.fillRect(230, 60, 30, 30, ILI9341_BLACK);
      tft.drawRect(230, 60, 30, 30, ILI9341_RED);
      tft.setTextColor(ILI9341_RED);
      tft.println("%");
      break;

    }

     if (endpt == true) {    // ends the loop if Contrast dB is pressed 
      c =2;
      break;
    }
    
    c =2;
    delay(100);
  }



  /*----------------------------------------------------------------------------------------------------------------------*/ 


  while (p.x >= 105 && p.x <= 165 && p.y >= 61 && p.y <= 200) {  /* Exticantion Ratio %  */
  
    
    getdata(data, SIZE);                // Getting DATA
    boolean endpt = false;
    boolean zero = false;

    float contrast = (MAX - MIN) / (MAX + MIN);
    float pcontrast = 100 * contrast;
    float dbcontrast = (-10) * log10(contrast);
    
    float pexratio = 100 * (MIN / MAX);

    float pdop = 100 - pcontrast;               
    float dbdop = (-10) * log10(1 - contrast);

    while (looop == 1) {          //graphics --- runs the loop one time 

      tft.setCursor(263, 143);                       // Reseting dB box to white
      tft.fillRect(260, 120, 35, 30, ILI9341_BLACK);
      tft.drawRect(260, 120, 35, 30, ILI9341_WHITE);
      tft.setTextColor(ILI9341_WHITE);
      tft.println("dB");


      tft.setCursor(233, 143);                    // selecting the % box red
      tft.drawRect(230, 120, 30, 30, ILI9341_BLACK);
      tft.drawRect(230, 120, 30, 30, ILI9341_RED);
      tft.setTextColor(ILI9341_RED);
      tft.println("%");
      looop = looop + 1;
    }


    tft.setTextColor(ILI9341_WHITE);             // printing the number
    tft.setCursor(141, 143);
    tft.fillRect(140, 120, 71, 30, ILI9341_BLACK);
    //    tft.drawRect(140, 120, 60, 30, ILI9341_WHITE);
    tft.println(pexratio, 2);

/*                                   Setting the setup as it was before                                */
   
    while (c == 0 || c == 1 ) {  // contrast %

      tft.setTextColor(ILI9341_WHITE);            // printing the % contrast
      tft.setCursor(141, 83);
      tft.fillRect(140, 60, 71, 30, ILI9341_BLACK);
      //    tft.drawRect(140, 60, 60, 30, ILI9341_WHITE);
      tft.println(pcontrast, 2);
      break;
    }
    while (c == 2) {            // contrast dB

      tft.setTextColor(ILI9341_WHITE);                      // printing contrast in dB
      tft.setCursor(141, 83);
      tft.fillRect(140, 60, 71, 30, ILI9341_BLACK);
      //    tft.drawRect(140, 60, 60, 30, ILI9341_WHITE);
      tft.println(dbcontrast, 2);
      break;
    }

    while (d == 1 || d == 0) { // DOP  %
      tft.setTextColor(ILI9341_WHITE);             // printing the number
      tft.setCursor(141, 203);
      tft.fillRect(140, 180, 71, 30, ILI9341_BLACK);
      //    tft.drawRect(140, 180, 60, 30, ILI9341_WHITE);
      tft.println(pdop);
      break;
    }

    while ( d == 2) {         // DOP dB

      tft.setTextColor(ILI9341_WHITE);
      tft.setCursor(141, 203);
      tft.fillRect(140, 180, 71, 30, ILI9341_BLACK);
      // tft.drawRect(140, 180, 60, 30, ILI9341_WHITE);
      tft.println(dbdop, 2);
      break;
    }

                                   /* Getting points */

    TS_Point p = ctp.getPoint();
    Serial.print("("); Serial.print(p.x);
    Serial.print(", "); Serial.print(p.y);
    Serial.println(")");


    while (p.x >= 30 && p.x <= 105 && p.y >= 61 && p.y <= 200) { // contrast %
      endpt = true;
      tft.setCursor(263, 83);                       // Reseting dB box to white
      tft.fillRect(260, 60, 35, 30, ILI9341_BLACK);
      tft.drawRect(260, 60, 35, 30, ILI9341_WHITE);
      tft.setTextColor(ILI9341_WHITE);
      tft.println("dB");


      tft.setCursor(233, 83);                    // selecting the % box red
      tft.fillRect(230, 60, 30, 30, ILI9341_BLACK);
      tft.drawRect(230, 60, 30, 30, ILI9341_RED);
      tft.setTextColor(ILI9341_RED);
      tft.println("%");
      c = 1;
      break;
    }

    while (p.x >= 30 && p.x <= 105 && p.y >= 1 && p.y <= 60) { // contrast dB
      endpt = true;
      tft.setCursor(233, 83);                    // resetting the % box to white
      tft.fillRect(230, 60, 30, 30, ILI9341_BLACK);
      tft.drawRect(230, 60, 30, 30, ILI9341_WHITE);
      tft.setTextColor(ILI9341_WHITE);
      tft.println("%");

      tft.setCursor(263, 83);                       // selecting dB box red
      tft.fillRect(260, 60, 35, 30, ILI9341_BLACK);
      tft.drawRect(260, 60, 35, 30, ILI9341_RED);
      tft.setTextColor(ILI9341_RED);
      tft.println("dB");
      break;
    }


    while (p.x > 165 && p.x <= 240 && p.y >= 61 && p.y <= 200) {  // DOP %
      endpt = true;
      tft.setCursor(263, 203);                       // Reseting dB box to white
      tft.fillRect(260, 180, 35, 30, ILI9341_BLACK);
      tft.drawRect(260, 180, 35, 30, ILI9341_WHITE);
      tft.setTextColor(ILI9341_WHITE);
      tft.println("dB");


      tft.setCursor(233, 203);                    // selecting the % box red
      tft.fillRect(230, 180, 30, 30, ILI9341_BLACK);
      tft.drawRect(230, 180, 30, 30, ILI9341_RED);
      tft.setTextColor(ILI9341_RED);
      tft.println("%");
      break;
    }
    while (p.x > 165 && p.x <= 240 && p.y >= 1  && p.y <= 60) { // DOP dB

      endpt = true;
      tft.setCursor(233, 203);                    // resetting the % box to white
      tft.fillRect(230, 180, 30, 30, ILI9341_BLACK);
      tft.drawRect(230, 180, 30, 30, ILI9341_WHITE);
      tft.setTextColor(ILI9341_WHITE);
      tft.println("%");

      tft.setCursor(263, 203);                       // selecting dB box red
      tft.fillRect(260, 180, 35, 30, ILI9341_BLACK);
      tft.drawRect(260, 180, 35, 30, ILI9341_RED);
      tft.setTextColor(ILI9341_RED);
      tft.println("dB");
      break;
    }

    while (p.x >= 120 && p.x <= 150 && p.y >= 15 && p.y <= 60) { // Extinction ratio dB 
      endpt = true;
      //Serial.println("LOOP IS WORKING");
      tft.setCursor(233, 143);                    // resetting the % box to white
      tft.fillRect(230, 120, 30, 30, ILI9341_BLACK);
      tft.drawRect(230, 120, 30, 30, ILI9341_WHITE);
      tft.setTextColor(ILI9341_WHITE);
      tft.println("%");

      tft.setCursor(263, 143);                       // selecting dB box red
      tft.fillRect(260, 120, 35, 30, ILI9341_BLACK);
      tft.drawRect(260, 120, 35, 30, ILI9341_RED);
      tft.setTextColor(ILI9341_RED);
      tft.println("dB");
      break;
    }


    if (endpt == true) {   // ends the loop if extinction ratio dB is pressed   
      e = 1;
      break;
    }
    
    e = 1;
    delay(100);
  } 

  while (p.x > 105 && p.x <= 165 && p.y >= 1 && p.y <= 60) {    /* Extinction  Ratio db  */
   
    getdata(data, SIZE);                // Getting DATA
    boolean endpt = false;
    boolean zero = false;
    float MAX = getmax(data, SIZE);
    float MIN = getmin(data, SIZE);


    float contrast = (MAX - MIN) / (MAX + MIN);
    float pcontrast = 100 * contrast;
    float dbcontrast = (-10) * log10(contrast);

    float dbexratio = (-10) * log10(MIN / MAX);

    float pdop = 100 - pcontrast;                       
    float dbdop = (-10) * log10(1 - (contrast));

    while (looop == 1) {              // graphics --- runs the loop one time 
      tft.setCursor(233, 143);                    // resetting the % box to white
      tft.fillRect(230, 120, 30, 30, ILI9341_BLACK);
      tft.drawRect(230, 120, 30, 30, ILI9341_WHITE);
      tft.setTextColor(ILI9341_WHITE);
      tft.println("%");

      tft.setCursor(263, 143);                       // selecting dB box red
      tft.fillRect(260, 120, 35, 30, ILI9341_BLACK);
      tft.drawRect(260, 120, 35, 30, ILI9341_RED);
      tft.setTextColor(ILI9341_RED);
      tft.println("dB");
      looop = looop + 1;
    }


    tft.setTextColor(ILI9341_WHITE);  // displaying extinction ratio dB
    tft.setCursor(141, 143);
    tft.fillRect(140, 120, 71, 30, ILI9341_BLACK);
    //    tft.drawRect(140, 120, 60, 30, ILI9341_WHITE);
    tft.println(dbexratio, 2);
/*                                                  Setting the setup as it was before                            */
    while (c == 0 || c == 1 ) {   // Contrast in %

      tft.setTextColor(ILI9341_WHITE);            // printing the % contrast
      tft.setCursor(141, 83);
      tft.fillRect(140, 60, 71, 30, ILI9341_BLACK);
      //    tft.drawRect(140, 60, 60, 30, ILI9341_WHITE);
      tft.println(pcontrast, 2);
      break;
    }
    
    while (c == 2) {             // Contrast in dB        

      tft.setTextColor(ILI9341_WHITE);                      // printing contrast in dB
      tft.setCursor(141, 83);
      tft.fillRect(140, 60, 71, 30, ILI9341_BLACK);
      //    tft.drawRect(140, 60, 60, 30, ILI9341_WHITE);
      tft.println(dbcontrast, 2);
      break;
    }

    while (d == 1 || d == 0) {   // DOP in %
      tft.setTextColor(ILI9341_WHITE);             // printing the number
      tft.setCursor(141, 203);
      tft.fillRect(140, 180, 71, 30, ILI9341_BLACK);
      //    tft.drawRect(140, 180, 60, 30, ILI9341_WHITE);
      tft.println(pdop);
      break;
    }

    while ( d == 2) {           // DOP in dB

      tft.setTextColor(ILI9341_WHITE);
      tft.setCursor(141, 203);
      tft.fillRect(140, 180, 71, 30, ILI9341_BLACK);
      // tft.drawRect(140, 180, 60, 30, ILI9341_WHITE);
      tft.println(dbdop, 2);
      break;
    }


                    /* Getting Points */
    TS_Point p = ctp.getPoint();
    Serial.print("("); Serial.print(p.x);
    Serial.print(", "); Serial.print(p.y);
    Serial.println(")");


    
    

    while (p.x >= 30 && p.x <= 105 && p.y >= 61 && p.y <= 200) {  // contrast %
      endpt = true;
      tft.setCursor(263, 83);                       // Reseting dB box to white
      tft.fillRect(260, 60, 35, 30, ILI9341_BLACK);
      tft.drawRect(260, 60, 35, 30, ILI9341_WHITE);
      tft.setTextColor(ILI9341_WHITE);
      tft.println("dB");


      tft.setCursor(233, 83);                    // selecting the % box red
      tft.fillRect(230, 60, 30, 30, ILI9341_BLACK);
      tft.drawRect(230, 60, 30, 30, ILI9341_RED);
      tft.setTextColor(ILI9341_RED);
      tft.println("%");
      c = 1;
      break;
    }

    while (p.x >= 30 && p.x <= 105 && p.y >= 1 && p.y <= 60) {    // contrast dB
      endpt = true;
      tft.setCursor(233, 83);                    // resetting the % box to white
      tft.fillRect(230, 60, 30, 30, ILI9341_BLACK);
      tft.drawRect(230, 60, 30, 30, ILI9341_WHITE);
      tft.setTextColor(ILI9341_WHITE);
      tft.println("%");

      tft.setCursor(263, 83);                       // selecting dB box red
      tft.fillRect(260, 60, 35, 30, ILI9341_BLACK);
      tft.drawRect(260, 60, 35, 30, ILI9341_RED);
      tft.setTextColor(ILI9341_RED);
      tft.println("dB");
      break;
    }


    while (p.x > 165 && p.x <= 240 && p.y >= 61 && p.y <= 200) {  // DOP %
      endpt = true;
      tft.setCursor(263, 203);                       // Reseting dB box to white
      tft.fillRect(260, 180, 35, 30, ILI9341_BLACK);
      tft.drawRect(260, 180, 35, 30, ILI9341_WHITE);
      tft.setTextColor(ILI9341_WHITE);
      tft.println("dB");


      tft.setCursor(233, 203);                    // selecting the % box red
      tft.fillRect(230, 180, 30, 30, ILI9341_BLACK);
      tft.drawRect(230, 180, 30, 30, ILI9341_RED);
      tft.setTextColor(ILI9341_RED);
      tft.println("%");
      break;
    }
    while (p.x > 165 && p.x <= 240 && p.y >= 1  && p.y <= 60) {   // DOP dB

      endpt = true;
      tft.setCursor(233, 203);                    // resetting the % box to white
      tft.fillRect(230, 180, 30, 30, ILI9341_BLACK);
      tft.drawRect(230, 180, 30, 30, ILI9341_WHITE);
      tft.setTextColor(ILI9341_WHITE);
      tft.println("%");

      tft.setCursor(263, 203);                       // selecting dB box red
      tft.fillRect(260, 180, 35, 30, ILI9341_BLACK);
      tft.drawRect(260, 180, 35, 30, ILI9341_RED);
      tft.setTextColor(ILI9341_RED);
      tft.println("dB");
      break;
    }



    while (p.x >= 105 && p.x <= 165 && p.y >= 61 && p.y <= 200) {  // Extinction Ratio in %
      endpt = true;
      //Serial.println("LOOP IS WORKING");
      tft.setCursor(263, 143);                       // Reseting dB box to white
      tft.fillRect(260, 120, 35, 30, ILI9341_BLACK);
      tft.drawRect(260, 120, 35, 30, ILI9341_WHITE);
      tft.setTextColor(ILI9341_WHITE);
      tft.println("dB");


      tft.setCursor(233, 143);                    // selecting the % box red
      tft.drawRect(230, 120, 30, 30, ILI9341_BLACK);
      tft.drawRect(230, 120, 30, 30, ILI9341_RED);
      tft.setTextColor(ILI9341_RED);
      tft.println("%");
      break;
    }

    if (endpt == true) {      // ends the loop if extinction ratio % is pressed 
      e = 2;
      break;
    }
    
    e = 2;
    delay(100);
  }




  /* ---------------------------------------------------------------------------------------------------------------------*/



  while  (p.x > 165 && p.x <= 240 && p.y >= 61 && p.y <= 200) {   /* DOP % */

    getdata(data, SIZE);                // Getting DATA
    boolean endpt = false;
    boolean zero = false;
    float MAX = getmax(data, SIZE);
    float MIN = getmin(data, SIZE);


    float contrast = (MAX - MIN) / (MAX + MIN);
    float pcontrast = 100 * contrast;
    float dbcontrast = (-10) * log10(contrast);

    float pexratio = 100 * (MIN / MAX);
    float dbexratio = (-10) * log10(MIN / MAX);


    float pdop = 100 - pcontrast;             



    while (looop == 1) {                              // graphics --- runs the loop one time
      tft.setCursor(263, 203);                       // Reseting dB box to white
      tft.fillRect(260, 180, 35, 30, ILI9341_BLACK);
      tft.drawRect(260, 180, 35, 30, ILI9341_WHITE);
      tft.setTextColor(ILI9341_WHITE);
      tft.println("dB");


      tft.setCursor(233, 203);                    // selecting the % box red
      tft.fillRect(230, 180, 30, 30, ILI9341_BLACK);
      tft.drawRect(230, 180, 30, 30, ILI9341_RED);
      tft.setTextColor(ILI9341_RED);
      tft.println("%");
      looop = looop + 1;
    }

    tft.setTextColor(ILI9341_WHITE);             // printing DOP in %
    tft.setCursor(141, 203);
    tft.fillRect(140, 180, 71, 30, ILI9341_BLACK);
    //    tft.drawRect(140, 180, 60, 30, ILI9341_WHITE);
    tft.println(pdop);

  /*                                           Setting the setup as it was before                                  */      
  
    while (e == 2 || e == 0) {          // Extinction ratio db
      tft.setTextColor(ILI9341_WHITE);     // db exratio
      tft.setCursor(141, 143);
      tft.fillRect(140, 120, 71, 30, ILI9341_BLACK);
      //    tft.drawRect(140, 120, 60, 30, ILI9341_WHITE);
      tft.println(dbexratio, 2);
      break;
    }

    while (e == 1) {                   // Extinction ratio %
      tft.setTextColor(ILI9341_WHITE);             // printing the number
      tft.setCursor(141, 143);
      tft.fillRect(140, 120, 71, 30, ILI9341_BLACK);
      //    tft.drawRect(140, 120, 60, 30, ILI9341_WHITE);
      tft.println(pexratio, 2);
      break;
    }

    while (c == 0 || c == 1 ) {        // Contrast %

      tft.setTextColor(ILI9341_WHITE);            // printing the % contrast
      tft.setCursor(141, 83);
      tft.fillRect(140, 60, 71, 30, ILI9341_BLACK);
      //    tft.drawRect(140, 60, 60, 30, ILI9341_WHITE);
      tft.println(pcontrast, 2);
      break;
    }
    
    while (c == 2) {                   // Contrast db

      tft.setTextColor(ILI9341_WHITE);                      // printing contrast in dB
      tft.setCursor(141, 83);
      tft.fillRect(140, 60, 71, 30, ILI9341_BLACK);
      //    tft.drawRect(140, 60, 60, 30, ILI9341_WHITE);
      tft.println(dbcontrast, 2);
      break;
    }

                      /* Getting points */
    TS_Point p = ctp.getPoint();
    Serial.print("("); Serial.print(p.x);
    Serial.print(", "); Serial.print(p.y);
    Serial.println(")");

    while (p.x >= 105 && p.x <= 165 && p.y >= 61 && p.y <= 200) {   // Extinction ratio %


      tft.setCursor(263, 143);                       // Reseting dB box to white
      tft.fillRect(260, 120, 35, 30, ILI9341_BLACK);
      tft.drawRect(260, 120, 35, 30, ILI9341_WHITE);
      tft.setTextColor(ILI9341_WHITE);
      tft.println("dB");


      tft.setCursor(233, 143);                    // selecting the % box red
      tft.drawRect(230, 120, 30, 30, ILI9341_BLACK);
      tft.drawRect(230, 120, 30, 30, ILI9341_RED);
      tft.setTextColor(ILI9341_RED);
      tft.println("%");
      e = 1;
      endpt = true;
      break;
    }

    while (p.x >= 105 && p.x <= 165 && p.y >= 1 && p.y <= 60) {    // Extinction  ratio db
      tft.setCursor(233, 143);                    // resetting the % box to white
      tft.fillRect(230, 120, 30, 30, ILI9341_BLACK);
      tft.drawRect(230, 120, 30, 30, ILI9341_WHITE);
      tft.setTextColor(ILI9341_WHITE);
      tft.println("%");

      tft.setCursor(263, 143);                       // selecting dB box red
      tft.fillRect(260, 120, 35, 30, ILI9341_BLACK);
      tft.drawRect(260, 120, 35, 30, ILI9341_RED);
      tft.setTextColor(ILI9341_RED);
      tft.println("dB");
      e = 2;
      endpt = true;
      break;
    }

    while (p.x >= 30 && p.x <= 105 && p.y >= 61 && p.y <= 200) {  // contrast %
      endpt = true;
      tft.setCursor(263, 83);                       // Reseting dB box to white
      tft.fillRect(260, 60, 35, 30, ILI9341_BLACK);
      tft.drawRect(260, 60, 35, 30, ILI9341_WHITE);
      tft.setTextColor(ILI9341_WHITE);
      tft.println("dB");


      tft.setCursor(233, 83);                    // selecting the % box red
      tft.fillRect(230, 60, 30, 30, ILI9341_BLACK);
      tft.drawRect(230, 60, 30, 30, ILI9341_RED);
      tft.setTextColor(ILI9341_RED);
      tft.println("%");
      c = 1;
      break;
    }

    while (p.x >= 30 && p.x <= 105 && p.y >= 1 && p.y <= 60) {    // contrast dB
      endpt = true;
      tft.setCursor(233, 83);                    // resetting the % box to white
      tft.fillRect(230, 60, 30, 30, ILI9341_BLACK);
      tft.drawRect(230, 60, 30, 30, ILI9341_WHITE);
      tft.setTextColor(ILI9341_WHITE);
      tft.println("%");

      tft.setCursor(263, 83);                       // selecting dB box red
      tft.fillRect(260, 60, 35, 30, ILI9341_BLACK);
      tft.drawRect(260, 60, 35, 30, ILI9341_RED);
      tft.setTextColor(ILI9341_RED);
      tft.println("dB");
      break;
    }

    while (p.x > 165 && p.x <= 240 && p.y >= 1  && p.y <= 60) {   // DOP dB 
      endpt = true;
      //Serial.println("LOOP IS WORKING");
      tft.setCursor(233, 203);                    // resetting the % box to white
      tft.fillRect(230, 180, 30, 30, ILI9341_BLACK);
      tft.drawRect(230, 180, 30, 30, ILI9341_WHITE);
      tft.setTextColor(ILI9341_WHITE);
      tft.println("%");

      tft.setCursor(263, 203);                       // selecting dB box red
      tft.fillRect(260, 180, 35, 30, ILI9341_BLACK);
      tft.drawRect(260, 180, 35, 30, ILI9341_RED);
      tft.setTextColor(ILI9341_RED);
      tft.println("dB");
      break;
    }

    if (endpt == true) {  //ends the loop if DOP dB is pressed
      d = 1;
      break;
    }

    d = 1;
    delay(100);
  }

  while (p.x > 165 && p.x <= 240 && p.y >= 1  && p.y <= 60) {    /* DOP db */

    getdata(data, SIZE);                              // Getting DATA
    boolean endpt = false;
    boolean zero = false;
    float MAX = getmax(data, SIZE);
    float MIN = getmin(data, SIZE);

    float contrast = (MAX - MIN) / (MAX + MIN);     
    float pcontrast = 100 * contrast;
    float dbcontrast = (-10) * log10(contrast);

    float pexratio = 100 * (MIN / MAX);
    float dbexratio = (-10) * log10(MIN / MAX);

    float dbdop = (-10) * log10(1 - (contrast));     


    while (looop == 1) {                              // graphics ---  runs the loop one time
      tft.setCursor(233, 203);                        // resetting the % box to white
      tft.fillRect(230, 180, 30, 30, ILI9341_BLACK);
      tft.drawRect(230, 180, 30, 30, ILI9341_WHITE);
      tft.setTextColor(ILI9341_WHITE);
      tft.println("%");

      tft.setCursor(263, 203);                       // selecting dB box red
      tft.fillRect(260, 180, 35, 30, ILI9341_BLACK);
      tft.drawRect(260, 180, 35, 30, ILI9341_RED);
      tft.setTextColor(ILI9341_RED);
      tft.println("dB");
      looop = looop + 1;
    }


    tft.setTextColor(ILI9341_WHITE);                  // displaying the DOP in dB
    tft.setCursor(141, 203);
    tft.fillRect(140, 180, 71, 30, ILI9341_BLACK);
    // tft.drawRect(140, 180, 60, 30, ILI9341_WHITE);
    tft.println(dbdop, 2);

  /*                                           Setting the setup as it was before                                  */                                                  
    while (e == 2 || e == 0) {                      //  extinction ratio  in dB       
      tft.setTextColor(ILI9341_WHITE);     // db exratio
      tft.setCursor(141, 143);
      tft.fillRect(140, 120, 71, 30, ILI9341_BLACK);
      //    tft.drawRect(140, 120, 60, 30, ILI9341_WHITE);
      tft.println(dbexratio, 2);
      break;
    }

    while (e == 1) {                               //   extinction ratio  in %  
      tft.setTextColor(ILI9341_WHITE);             // printing the number
      tft.setCursor(141, 143);
      tft.fillRect(140, 120, 71, 30, ILI9341_BLACK);
      //    tft.drawRect(140, 120, 60, 30, ILI9341_WHITE);
      tft.println(pexratio, 2);
      break;
    }

    while (c == 0 || c == 1 ) {                   //    contrast in % 

      tft.setTextColor(ILI9341_WHITE);            // printing the % contrast
      tft.setCursor(141, 83);
      tft.fillRect(140, 60, 71, 30, ILI9341_BLACK);
      //    tft.drawRect(140, 60, 60, 30, ILI9341_WHITE);
      tft.println(pcontrast, 2);
      break;
    }
    
    while (c == 2) {                              // contrast in dB 

      tft.setTextColor(ILI9341_WHITE);                      // printing contrast in dB
      tft.setCursor(141, 83);
      tft.fillRect(140, 60, 71, 30, ILI9341_BLACK);
      //    tft.drawRect(140, 60, 60, 30, ILI9341_WHITE);
      tft.println(dbcontrast, 2);
      break;
    }

 
    
    /* Getting Points */
    
    TS_Point p = ctp.getPoint();
    Serial.print("("); Serial.print(p.x);
    Serial.print(", "); Serial.print(p.y);
    Serial.println(")");


 

    while (p.x >= 105 && p.x <= 165 && p.y >= 61 && p.y <= 200) {       // Extinction Ratio  %


      tft.setCursor(263, 143);                       // Reseting dB box to white
      tft.fillRect(260, 120, 35, 30, ILI9341_BLACK);
      tft.drawRect(260, 120, 35, 30, ILI9341_WHITE);
      tft.setTextColor(ILI9341_WHITE);
      tft.println("dB");


      tft.setCursor(233, 143);                    // selecting the % box red
      tft.drawRect(230, 120, 30, 30, ILI9341_BLACK);
      tft.drawRect(230, 120, 30, 30, ILI9341_RED);
      tft.setTextColor(ILI9341_RED);
      tft.println("%");
      e = 1;
      endpt = true;
      break;
    }

    while (p.x >= 105 && p.x <= 165 && p.y >= 1 && p.y <= 60) {        // Extinction  Ratio db
      tft.setCursor(233, 143);                    // resetting the % box to white
      tft.fillRect(230, 120, 30, 30, ILI9341_BLACK);
      tft.drawRect(230, 120, 30, 30, ILI9341_WHITE);
      tft.setTextColor(ILI9341_WHITE);
      tft.println("%");

      tft.setCursor(263, 143);                       // selecting dB box red
      tft.fillRect(260, 120, 35, 30, ILI9341_BLACK);
      tft.drawRect(260, 120, 35, 30, ILI9341_RED);
      tft.setTextColor(ILI9341_RED);
      tft.println("dB");
      e = 2;
      endpt = true;
      break;
    }

    while (p.x >= 30 && p.x <= 105 && p.y >= 61 && p.y <= 200) {       // Contrast %
      endpt = true;
      tft.setCursor(263, 83);                       // Reseting dB box to white
      tft.fillRect(260, 60, 35, 30, ILI9341_BLACK);
      tft.drawRect(260, 60, 35, 30, ILI9341_WHITE);
      tft.setTextColor(ILI9341_WHITE);
      tft.println("dB");


      tft.setCursor(233, 83);                    // selecting the % box red
      tft.fillRect(230, 60, 30, 30, ILI9341_BLACK);
      tft.drawRect(230, 60, 30, 30, ILI9341_RED);
      tft.setTextColor(ILI9341_RED);
      tft.println("%");
      c = 1;
      break;
    }

    while (p.x >= 30 && p.x <= 105 && p.y >= 1 && p.y <= 60) {        // Contrast dB
      endpt = true;
      tft.setCursor(233, 83);                    // resetting the % box to white
      tft.fillRect(230, 60, 30, 30, ILI9341_BLACK);
      tft.drawRect(230, 60, 30, 30, ILI9341_WHITE);
      tft.setTextColor(ILI9341_WHITE);
      tft.println("%");

      tft.setCursor(263, 83);                       // selecting dB box red
      tft.fillRect(260, 60, 35, 30, ILI9341_BLACK);
      tft.drawRect(260, 60, 35, 30, ILI9341_RED);
      tft.setTextColor(ILI9341_RED);
      tft.println("dB");
      break;
    }

    while (p.x > 165 && p.x <= 230 && p.y >= 61 && p.y <= 200) {      // DOP % 
      endpt = true;
      //Serial.println("LOOP IS WORKING");
      tft.setCursor(263, 203);                       // Reseting dB box to white
      tft.fillRect(260, 180, 35, 30, ILI9341_BLACK);
      tft.drawRect(260, 180, 35, 30, ILI9341_WHITE);
      tft.setTextColor(ILI9341_WHITE);
      tft.println("dB");


      tft.setCursor(233, 203);                    // selecting the % box red
      tft.fillRect(230, 180, 30, 30, ILI9341_BLACK);
      tft.drawRect(230, 180, 30, 30, ILI9341_RED);
      tft.setTextColor(ILI9341_RED);
      tft.println("%");
      break;
    }

    if (endpt == true) {        // ends the loop if DOP % is pressed 
      d = 2;
      break;
    }

    d = 2;
    delay(100);
  }



  /*----------------------------------------------------------------------------------------------------------------------*/


  delay(100);
} 


// end of loop()


/*------------------------------------------------------------------------------------------------------------------------*/

void getdata(float data[], int Size) {

  for (int i = 0; i < Size; i++) {
    float v2 = (analogRead(15) * maxvoltage) / 1023.0;
    float vtot = v2 * (R1 + R2) / R2;
    data[i] = vtot;
//    Serial.print("Votlage Total is ");
    Serial.println(vtot);
    
//    delay(10);
  }
}

/*-------------------------------------------------------------------------------------------------------------------------*/

float getmax(float data[], int Size) {
  float M;
  if (data[0] >= data[1]) {
    M = data[0];
  }
  else {
    M = data[1];
  }
  for (int i = 0; i < Size; i++) {
    if (M <= data[i]) {
      M = data[i];
    }
    else {
      M = M;
    }
  }
  return M;
}

/*-------------------------------------------------------------------------------------------------------------------------*/

float getmin(float data[], int Size) {
  float m;
  if (data[0] <= data[1] && data[0]!=0) {
    m = data[0];
  }
  else {
    m = data[1];
  }
  for (int i = 0; i < Size; i++) {

    if (m >= data[i] && data[i] !=0 ) {
      m = data[i];
    }
    else {
      m = m;
    }
  }

  return m;
}




