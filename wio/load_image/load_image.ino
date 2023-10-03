#include <stdint.h>
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
  img->draw(x, y);
  img->release();
}



void setup() {
  if (!SD.begin(SDCARD_SS_PIN, SDCARD_SPI)) {
    Serial.println("SD card error!\n");
    while (1);
  }

  tft.begin();
  tft.setRotation(3);

  delay(1000);
  drawImage<uint8_t>("kui.bmp", 0, 0);
}

void loop() {
  // do nothing
}
