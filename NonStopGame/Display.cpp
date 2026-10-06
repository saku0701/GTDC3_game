#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

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

// ========================================
// LCDの前回表示値
// ========================================

// 前回表示した現在ゲーム番号
static int previousCurrentGame = -1;

// 前回表示した全ゲーム数
static int previousTotalGame = -1;

// 前回表示した残り時間バーの長さ
static int previousRemainingBar = -1;

// ディスプレイ初期化
void initDisplay()
{
    if(!oled.begin( SSD1306_SWITCHCAPVCC, 0x3C)){
        while(true);
    }

    oled.clearDisplay();
    oled.display();

    lcd.init();
    lcd.backlight();
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

// メニュー表示
void showMenuScreen(Difficulty difficulty){
    oled.clearDisplay();
    oled.setTextSize(1);

    oled.setCursor(0,0);
    oled.println("NON STOP GAME");

    oled.setCursor(0,20);

    switch(difficulty){
        case EASY:
            oled.println("EASY");
            break;

        case NORMAL:
            oled.println("NORMAL");
            break;

        case HARD:
            oled.println("HARD");
            break;
    }

    oled.setCursor(0,50);
    oled.println("CENTER START");

    oled.display();
}

// カウントダウン表示
void showCountdown(int count){
    oled.clearDisplay();
    oled.setTextSize(3);
    oled.setCursor(50,20);
    oled.print(count);
    oled.display();
}

// ゲームクリア表示
void showGameClear(){
    oled.clearDisplay();
    oled.setTextSize(2);
    oled.setCursor(0,20);
    oled.println("CLEAR!");
    oled.display();
}

// ゲーム数と残り時間をLCDに表示する
void showGameStatus(int currentGame, int totalGame, unsigned long elapsed, unsigned long limit){
    // 制限時間が0の場合、0除算を防止する
    if(limit == 0){
        return;
    }

    // 残り時間の割合を計算する
    float remainingRatio = 1.0f - ((float)elapsed / (float)limit);

    // 残り時間が0%未満にならないように補正する
    if(remainingRatio < 0.0f){
        remainingRatio = 0.0f;
    }

    // 残り時間が100%を超えないように補正する
    if(remainingRatio > 1.0f){
        remainingRatio = 1.0f;
    }

    // LCDの横幅16文字に合わせてバーの長さを計算する
    int remainingBar = (int)(remainingRatio * 16.0f);

    // ゲーム数表示に変更があった場合だけ1行目を更新する
    if(currentGame != previousCurrentGame || totalGame != previousTotalGame){
        // 1行目を16文字の空白で上書きする
        lcd.setCursor(0, 0);
        lcd.print("                ");

        // カーソルを1行目の先頭へ戻す
        lcd.setCursor(0, 0);

        // 現在ゲーム数 / 全ゲーム数を表示する
        lcd.print(currentGame);
        lcd.print("/");
        lcd.print(totalGame);

        // 今回の値を前回値として保存する
        previousCurrentGame = currentGame;
        previousTotalGame = totalGame;
    }

    // 残り時間バーに変更があった場合だけ2行目を更新する
    if(remainingBar != previousRemainingBar){
        // 2行目を16文字の空白で上書きする
        lcd.setCursor(0, 1);
        lcd.print("                ");

        // カーソルを2行目の先頭へ戻す
        lcd.setCursor(0, 1);

        // 残り時間分だけバーを表示する
        for(int i = 0; i < remainingBar; i++){
            lcd.print("#");
        }

        // 今回の値を前回値として保存する
        previousRemainingBar = remainingBar;
    }
}

// LCDのゲーム進捗表示の履歴をリセットする
void resetGameStatusDisplay(){
    // 次回showGameStatus()呼び出し時に必ず全項目が更新されるようにする
    previousCurrentGame = -1;
    previousTotalGame = -1;
    previousRemainingBar = -1;

    // LCDの表示を一度だけ消去する
    lcd.clear();
}