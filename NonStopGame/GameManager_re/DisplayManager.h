/*
ディスプレイマネージャー
表示関数を条件に応じて呼び出す
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <LiquidCrystal_I2C.h>

#include "GameManager.h"

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
static bool isI2cDeviceConnected(uint8_t deviceAddress);

// ディスプレイ初期化
void initDisplay();

// 画面表示
void updateDisplay(
    GameState currentState, 
    MenuState currentMenuState, 
    static MiniGameType currentGame, 
    int menuIndex, 
    Difficulty difficulty, 
    int gameCount, 
    int speedIndex, 
    int count, 
    char* text);