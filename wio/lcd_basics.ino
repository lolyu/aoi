#include "TFT_eSPI.h"

TFT_eSPI tft;

void setup() {
  tft.begin();
  tft.setRotation(3);

  tft.fillScreen(TFT_LIGHTGREY);

  // draw a pixel
  tft.drawPixel(160, 120, TFT_RED);

  // draw a rectangle with four lines
  // (140, 100), (180, 100)
  // (140, 140), (180, 140)
  tft.drawLine(140, 100, 180, 100, TFT_RED);
  tft.drawLine(140, 140, 180, 140, TFT_RED);
  tft.drawLine(140, 100, 140, 140, TFT_RED);
  tft.drawLine(180, 100, 180, 140, TFT_RED);

  // draw a larger rectangle with horizontal && vertical lines
  // (120, 80), (200, 80)
  // (120, 160), (200, 160)
  tft.drawFastHLine(120, 80, 80, TFT_RED);
  tft.drawFastHLine(120, 160, 80, TFT_RED);
  tft.drawFastVLine(120, 80, 80, TFT_RED);
  tft.drawFastVLine(200, 80, 80, TFT_RED);

  // draw a rectangle
  tft.drawRect(100, 60, 120, 120, TFT_RED);

  // draw a circle
  tft.drawCircle(160, 120, 20, TFT_CYAN);

  // draw a ellipse
  tft.drawEllipse(160, 120, 100, 60, TFT_BLACK);

  // draw a triangle
  tft.drawTriangle(160, 20, 120, 160, 200, 160, TFT_BLACK);

  // draw a round rectangle
  tft.drawRoundRect(110, 70, 100, 100, 10, TFT_BLACK);

  // draw three characters
  tft.drawChar(140, 120, 'A', TFT_BLACK, TFT_RED, 2);  //Draw a black character A from (140,120)
  tft.drawChar(155, 120, 'B', TFT_BLACK, TFT_RED, 2);  //Draw a black character B from (155,120)
  tft.drawChar(170, 120, 'C', TFT_BLACK, TFT_RED, 2);  //Draw a black character

  // draw strings
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);     // text size is from 1 ~ 7
  tft.drawString("HelloWorld!", 0, 0);
  tft.drawString("HelloWorld!", 0, 20);
}

void loop() {
}
