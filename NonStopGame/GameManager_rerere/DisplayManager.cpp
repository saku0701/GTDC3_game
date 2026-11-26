#include "MiniGameManager.h"
#include "DisplayManager.h"
#include "LCDManager.h"
#include "OLEDManager.h"

extern bool leftTriggered;
extern bool centerTriggered;
extern bool rightTriggered;

// ========================================
// LCDの前回表示値
// ========================================
// 前回表示した現在ゲーム番号
extern int previousCurrentGame;

// 前回表示した全ゲーム数
extern int previousTotalGame;

// 前回表示した残り時間バーの長さ
extern int previousRemainingBar;


// PUSHゲームのボタン抽選用
// extern ButtonType targetButton;


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

// ディスプレイ初期化
bool initDisplay(){
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

    Serial.println("[DISPLAY] LCD initialization succeeded");

    Serial.println("[DISPLAY] All displays initialized");

    return true;
}


void updateDisplay(
    GameState currentState, 
    MenuState currentMenuState, 
    MiniGameType currentGame, 
    int menuIndex, 
    Difficulty difficulty, 
    int gameCount, 
    int speedIndex, 
    int count, 
    char* instruction,
    ButtonType targetButton, //追加
    ButtonType targetButtonInOrder[], //追加
    int targetDistance, //追加
    int currentDistance, //追加
    int targetAngle, //追加
    int currentAngle,
    int targetCount,
    int completedCount,
    int percent,
    int totalGame,
    unsigned long elapsed,
    unsigned long limit
    )
{
    switch (currentState)
    {
        case MENU_INIT:
            resetGameStatusDisplay();
            showMenuScreen(menuIndex);
            break;

        case MENU:
            switch (currentMenuState)
            {
                case MENU_MAIN:
                    showMenuScreen(menuIndex);
                    break;
                case MENU_DIFFICULTY:
                    showDifficultyMenu(difficulty);
                    break;
                case MENU_GAME_COUNT:
                    showGameCountMenu(gameCount);
                    break;
                case MENU_SPEED:
                    showSpeedMenu(speedIndex);
                    break;
            }
            break;

        case STATE_COUNTDOWN:
            showCountdown(count);
            break;
        
        case STATE_Instruction:
            showInstruction(instruction);
            break;

        case STATE_MINIGAME:
            switch (currentGame)
            {
                case GAME_PUSH:
                    showPushGame(targetButton, difficulty);
                    break;

                case GAME_PUSH_INORDER:
                    showPushInOrderGame(targetButtonInOrder, difficulty);
                    break;

                case GAME_JOYSTICK_DOWN:
                    showJoystickDownGame(targetAngle, currentAngle);
                    break;

                case GAME_DISTANCE_KEEP:
                    showDistanceGame(targetDistance, currentDistance);
                    break;

                case GAME_JOYSTICK_ROTATE:
                    showJoystickRotateGame(targetCount, completedCount, percent);
                    break;
            }
            showGameStatus(currentGame, totalGame, elapsed, limit);

        case STATE_SUCCESS:
            showSuccess();
            break;

        case STATE_GAMEOVER:
            showFailed();
            break;

        case STATE_GAMECLEAR:
            showGameClear();
            break;
    }
}