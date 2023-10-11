#include "seeed_line_chart.h"

const int max_data_count = 10;
doubles data;
TFT_eSPI tft;
TFT_eSprite spr = TFT_eSprite(&tft);


void setup() {
  tft.begin();
  tft.setRotation(3);
  spr.createSprite(tft.width(), tft.height());
}

void loop() {
  spr.fillSprite(TFT_LIGHTGREY);

  if (data.size() == max_data_count) {
    data.pop();
  }
  data.push(random(1, 10));

  auto header = text(0, 0);
  header.value("test");
  header.align(center);
  header.valign(vcenter);
  header.width(tft.width());
  header.thickness(3);
  header.height(header.font_height() * 2);
  header.draw();

  auto content = line_chart(20, header.height());
  content.height(tft.height() - header.height() * 1.5);
  // content.x(), content.y() is the topleft point
  content.width(tft.width() - content.x() * 2);
  content.based_on(0.0);
  content.show_circle(true);
  content.value(data);
  content.color(TFT_PURPLE);
  content.draw();

  spr.pushSprite(0, 0);

  delay(1000);
}
