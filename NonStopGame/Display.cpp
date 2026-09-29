#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "Display.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define TEXTSIZE_MIDDLE 2
#define TEXTSIZE_BIG 3


Adafruit_SSD1306 oled(
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    &Wire,
    -1
);

// ディスプレイ初期化
void initDisplay()
{
    if(!oled.begin(
        SSD1306_SWITCHCAPVCC,
        0x3C))
    {
        while(true);
    }

    oled.clearDisplay();
    oled.display();
}

// ミニゲーム指示表示
void showInstruction(
    const char* text)
{
    oled.clearDisplay();

    oled.setTextColor(WHITE);

    // 仮に2倍
    oled.setTextSize(TEXTSIZE_MIDDLE);

    oled.setCursor(10, 20);

    oled.println(text);

    oled.display();
}

// 
void showSuccess()
{
    oled.clearDisplay();

    oled.setTextSize(TEXTSIZE_BIG);
    oled.setCursor(15,20);

    oled.println("OK!");

    oled.display();
}

void showFailed()
{
    oled.clearDisplay();

    oled.setTextSize(TEXTSIZE_BIG);
    oled.setCursor(10,20);

    oled.println("MISS");

    oled.display();
}