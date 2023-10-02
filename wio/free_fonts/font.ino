#include "TFT_eSPI.h"

#include "Free_Fonts.h"
TFT_eSPI tft;


void setup() {
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(TFT_LIGHTGREY);

  tft.setTextColor(TFT_RED);

  tft.setFreeFont(&FreeMono9pt7b);
  tft.drawString("HelloWorld", 0, 0);

  tft.setFreeFont(FF25);
  tft.drawString("HelloWorld", 0, 100);

  tft.setFreeFont(FMB12);
  tft.drawString("HelloWorld", 0, 200);
}
void loop() {}
