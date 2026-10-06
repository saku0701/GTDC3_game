#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <LiquidCrystal_I2C.h>

#include "Display.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define TEXTSIZE_SMALL 1
#define TEXTSIZE_MIDDLE 2
#define TEXTSIZE_BIG 3

// I2Cアドレス
#define OLED_I2C_ADDRESS  0x3C
#define LCD_I2C_ADDRESS   0x27

#define LCD_COLUMNS 16
#define LCD_ROWS 2

// I2Cデバイス応答確認
// 指定されたI2Cアドレスにデバイスが応答するか確認する
static bool isI2cDeviceConnected(uint8_t deviceAddress){
    // 指定したアドレスへの通信を開始する
    Wire.beginTransmission(deviceAddress);

    // 通信を終了し、結果を取得する
    byte result = Wire.endTransmission();

    // 戻り値0の場合は正常応答
    return result == 0;
}

LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, LCD_COLUMNS, LCD_ROWS);

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
bool initDisplay(void){
    // I2C通信を開始する
    Wire.begin();

    Serial.println("[DISPLAY] Initialization start");

    // OLED接続確認
    if(!isI2cDeviceConnected(OLED_I2C_ADDRESS)){
        Serial.println("[ERROR] OLED is not responding");
        Serial.println("[ERROR] Check OLED power, GND, SDA and SCL");
        return false;
    }

    Serial.println("[DISPLAY] OLED detected");

    // OLED初期化
    if(!oled.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)){
        Serial.println("[ERROR] OLED initialization failed");
        return false;
    }

    // OLEDの初期表示条件を設定する
    oled.clearDisplay();
    oled.setTextColor(SSD1306_WHITE);
    oled.setTextSize(TEXTSIZE_SMALL);
    oled.display();

    Serial.println("[DISPLAY] OLED initialization succeeded");

    // LCD接続確認
    if(!isI2cDeviceConnected(LCD_I2C_ADDRESS)){
        Serial.println("[ERROR] LCD is not responding");
        Serial.println("[ERROR] Check LCD power, GND, SDA and SCL");
        return false;
    }

    Serial.println("[DISPLAY] LCD detected");

    // LCD初期化
    lcd.init();
    lcd.backlight();

    // カーソル表示と点滅を無効にする
    lcd.noCursor();
    lcd.noBlink();

    // LCDの初期表示を消去する
    lcd.clear();

    Serial.println(
        "[DISPLAY] LCD initialization succeeded");

    Serial.println(
        "[DISPLAY] All displays initialized");

    return true;
}

// ミニゲーム指示表示
void showInstruction(const char* text){
    oled.clearDisplay();

    oled.setTextColor(WHITE);

    // 仮に2倍
    oled.setTextSize(TEXTSIZE_MIDDLE);

    oled.setCursor(10, 20);

    oled.println(text);

    oled.display();
}

// ミニゲーム成功画面
void showSuccess(){
    oled.clearDisplay();

    oled.setTextSize(TEXTSIZE_BIG);
    oled.setCursor(15,20);

    oled.println("OK!");

    oled.display();
}

// ミニゲーム失敗画面
void showFailed(){
    oled.clearDisplay();

    oled.setTextSize(TEXTSIZE_BIG);
    oled.setCursor(10,20);

    oled.println("MISS");

    oled.display();
}

// ジョイスティック位置表示
void showJoystickInfo(int targetAngle, int currentAngle){
    oled.clearDisplay();

    oled.setTextColor(WHITE);

    oled.setTextSize(TEXTSIZE_SMALL);

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
void showDistanceInfo(int targetDistance, int currentDistance){
    oled.clearDisplay();

    oled.setTextColor(WHITE);
    oled.setTextSize(TEXTSIZE_SMALL);

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

    oled.setTextSize(TEXTSIZE_SMALL);

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
    oled.setTextSize(TEXTSIZE_SMALL);

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
    oled.setTextSize(TEXTSIZE_BIG);
    oled.setCursor(50,20);
    oled.print(count);
    oled.display();
}

// ゲームクリア表示
void showGameClear(){
    oled.clearDisplay();
    oled.setTextSize(TEXTSIZE_MIDDLE);
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

// ========================================
// メニュー画面
// ========================================
void showMainMenu(int menuIndex){
    oled.clearDisplay();

    oled.setTextColor(WHITE);
    oled.setTextSize(TEXTSIZE_SMALL);

    // タイトル
    oled.setCursor(14, 2);
    oled.println("NON STOP GAME");

    // 「EDIT」表示
    oled.setCursor(10, 25);

    if(menuIndex == 0){
        oled.print("> ");
    }else{
        oled.print("  ");
    }

    oled.println("EDIT");

    // 「START」表示
    oled.setCursor(10, 45);

    if(menuIndex == 1){
        oled.print("> ");
    }else{
        oled.print("  ");
    }

    oled.println("START");

    oled.display();
}

// ========================================
// 難易度選択画面
// ========================================
void showDifficultyMenu(Difficulty difficulty){
    oled.clearDisplay();

    oled.setTextColor(WHITE);
    oled.setTextSize(TEXTSIZE_SMALL);

    oled.setCursor(0, 2);
    oled.println("SELECT DIFFICULTY");

    oled.setTextSize(TEXTSIZE_MIDDLE);
    oled.setCursor(18, 25);

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

    oled.setTextSize(TEXTSIZE_SMALL);
    oled.setCursor(0, 55);
    oled.println("< > SELECT  OK NEXT");

    oled.display();
}

// ========================================
// ゲーム数選択画面
// ========================================
void showGameCountMenu(int gameCount){
    oled.clearDisplay();

    oled.setTextColor(WHITE);
    oled.setTextSize(TEXTSIZE_SMALL);

    oled.setCursor(0, 2);
    oled.println("SELECT GAME COUNT");

    oled.setTextSize(TEXTSIZE_BIG);
    oled.setCursor(52, 22);
    oled.println(gameCount);

    oled.setTextSize(TEXTSIZE_SMALL);
    oled.setCursor(0, 55);
    oled.println("< > CHANGE  OK NEXT");

    oled.display();
}

// ========================================
// スピード選択画面
// ========================================
void showSpeedMenu(int speedIndex){
    oled.clearDisplay();

    oled.setTextColor(WHITE);
    oled.setTextSize(TEXTSIZE_SMALL);

    oled.setCursor(0, 2);
    oled.println("SELECT SPEED");

    oled.setTextSize(TEXTSIZE_MIDDLE);
    oled.setCursor(10, 25);

    switch(speedIndex){
        case SPEED_SLOW:
            oled.println("SLOW");
            break;

        case SPEED_NORMAL:
            oled.println("NORMAL");
            break;

        case SPEED_FAST:
            oled.println("FAST");
            break;

        default:
            oled.println("NORMAL");
            break;
    }

    oled.setTextSize(TEXTSIZE_SMALL);
    oled.setCursor(0, 55);
    oled.println("< > SELECT  OK SET");

    oled.display();
}