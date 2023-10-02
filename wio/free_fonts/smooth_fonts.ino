#include <SPI.h>
#include "Seeed_FS.h"
#include "SD/Seeed_SD.h"
#include "TFT_eSPI.h"

TFT_eSPI tft;

void setup() {
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(TFT_LIGHTGREY);

  while (!SD.begin(SDCARD_SS_PIN, SDCARD_SPI)) {
    Serial.println("SD card error!\n");
    while (1)
      ;
  }

  delay(1000);

  tft.loadFont("ArialNarrow-24.vlw");
  tft.showFont(2000);
  tft.setTextSize(5);

  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.setCursor(0, 0);
  tft.println("KonniChiwa");
  tft.println("Sayonara");
  tft.println();

  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.println("你好");
  tft.println("欢迎使用！");
  tft.unloadFont();
}

void loop() {
}
