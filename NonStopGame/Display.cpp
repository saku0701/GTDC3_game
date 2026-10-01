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

// ミニゲーム成功画面
void showSuccess()
{
    oled.clearDisplay();

    oled.setTextSize(TEXTSIZE_BIG);
    oled.setCursor(15,20);

    oled.println("OK!");

    oled.display();
}

// ミニゲーム失敗画面
void showFailed()
{
    oled.clearDisplay();

    oled.setTextSize(TEXTSIZE_BIG);
    oled.setCursor(10,20);

    oled.println("MISS");

    oled.display();
}

// ジョイスティック位置表示
void showJoystickInfo(
    int targetAngle,
    int currentAngle)
{
    oled.clearDisplay();

    oled.setTextColor(WHITE);

    oled.setTextSize(1);

    oled.setCursor(0, 0);
    oled.print("TARGET");

    oled.setCursor(0, 10);
    oled.print(targetAngle);
    oled.print(" DEG");

    oled.setCursor(0, 35);
    oled.print("NOW");

    oled.setCursor(0, 45);
    oled.print(currentAngle);
    oled.print(" DEG");

    oled.display();
}

// 測距表示
void showDistanceInfo(int targetDistance, int currentDistance)
{
    oled.clearDisplay();

    oled.setTextColor(WHITE);
    oled.setTextSize(1);

    oled.setCursor(0, 0);
    oled.print("TARGET");

    oled.setCursor(0, 12);
    oled.print(targetDistance);
    oled.print(" cm");

    oled.setCursor(0, 36);
    oled.print("CURRENT");

    oled.setCursor(0, 48);
    oled.print(currentDistance);
    oled.print(" cm");

    oled.display();
}

// ジョイスティック回転表示
void showRotateInfo(int targetCount, int completedCount, int percent){
    oled.clearDisplay();

    oled.setTextSize(1);

    oled.setCursor(0,0);
    oled.print("TARGET:");
    oled.println(targetCount);

    oled.setCursor(0,20);
    oled.print("DONE:");
    oled.println(completedCount);

    oled.setCursor(0,40);
    oled.print("PROGRESS:");
    oled.print(percent);
    oled.println("%");

    oled.display();
}