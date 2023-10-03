#include <functional>
#include <stdint.h>
#include <string>
#include <SPI.h>
#include "Seeed_FS.h"
#include "SD/Seeed_SD.h"
#include "TFT_eSPI.h"

TFT_eSPI tft;

template<class type>
struct RawImage {
  type *ptr() {
    return (type *)(this + 1);
  }
  type get(int16_t x, int16_t y) {
    return this->ptr()[y * width() + x];
  }
  void draw(size_t x = 0, size_t y = 0) {
    tft.pushImage(x, y, width(), height(), ptr());
  }
  void release() {
    delete[] this;
  }
  int16_t width() {
    return _width;
  }
  int16_t height() {
    return _height;
  }
private:
  int16_t _width;
  int16_t _height;
};

typedef RawImage<uint8_t> Raw8;
typedef RawImage<uint16_t> Raw16;

template<class type>
RawImage<type> *newImage(const char *path) {
  typedef RawImage<type> raw;
  File f = SD.open(path, FILE_READ);
  if (!f) {
    Serial.println("Failed to load image!\n");
    return nullptr;
  }
  int32_t size = f.size();
  raw *mem = (raw *)new uint8_t[size];
  if (mem == nullptr) {
    return nullptr;
  }
  f.read(mem, size);
  f.close();
  return mem;
}

template<class type>
void drawImage(const char *path, size_t x = 0, size_t y = 0) {
  auto img = newImage<type>(path);
  if (!img) {
    return;
  }
  img->draw(x, y);
  img->release();
}

bool file_exists(const std::string &path) {
  return SD.exists(path.c_str());
}

void check_condition_false_then_pause(std::function<bool()> condition, const std::string &err_str) {
  if (!condition()) {
    Serial.println(err_str.c_str());
    tft.println(err_str.c_str());
    while (1) {
      delay(1000);
    }
  }
}


void setup() {
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(TFT_LIGHTGREY);

  // check if SD card is loaded
  check_condition_false_then_pause(
    []() {
      return SD.begin(SDCARD_SS_PIN, SDCARD_SPI);
    },
    "SD card error!");

  // check if font file is present
  std::string font_file = "fonts/ArialNarrow-24.vlw";
  check_condition_false_then_pause(
    [&font_file]() {
      return file_exists(font_file);
    },
    "Font loading error!");

  tft.loadFont(font_file.c_str());
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(90, 10);
  tft.print("Brightness");
  tft.setCursor(110, 90);
  tft.print("Volume");

  // draw shapes
  tft.drawRect(75, 45, 160, 20, TFT_NAVY);
  tft.fillRect(75, 45, 120, 20, TFT_NAVY);
  tft.fillCircle(35, 55, 25, TFT_RED);
  tft.fillCircle(275, 55, 25, TFT_DARKGREEN);
  tft.drawRect(75, 120, 160, 20, TFT_NAVY);
  tft.fillRect(75, 120, 90, 20, TFT_NAVY);
  tft.fillCircle(35, 130, 25, TFT_RED);
  tft.fillCircle(275, 130, 25, TFT_DARKGREEN);

  // draw images
  drawImage<uint8_t>("images/back.bmp", 0, 180);
  drawImage<uint8_t>("images/home.bmp", 260, 180);

  tft.unloadFont();
}

void loop() {
  // do nothing
}
