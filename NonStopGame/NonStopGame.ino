#include "MiniGame.h"
#include "MiniGameManager.h"
#include "Display.h"
#include "GameManager.h"

// ボタン入力ピン
#define BUTTON_LEFT_PIN       2
#define BUTTON_CENTER_PIN     3
#define BUTTON_RIGHT_PIN      4

// HC-SR04接続ピン
#define DISTANCE_ECHO_PIN     8
#define DISTANCE_TRIGGER_PIN  9

// メインループの更新周期
#define MAIN_LOOP_DELAY_MS   10UL

void setup(){
    Serial.begin(115200);

    // ボタン入力ピン設定
    pinMode(BUTTON_LEFT_PIN, INPUT_PULLUP);
    pinMode(BUTTON_CENTER_PIN, INPUT_PULLUP);
    pinMode(BUTTON_RIGHT_PIN, INPUT_PULLUP);

    // HC-SR04設定
    pinMode(DISTANCE_TRIGGER_PIN, OUTPUT);//TRIG
    pinMode(DISTANCE_ECHO_PIN, INPUT);//ECHO

    // 乱数初期化
    randomSeed(micros());

    // 表示デバイスを初期化
    bool displayInitialized = initDisplay();

    // OLEDまたはLCDの初期化に失敗した場合
    if(!displayInitialized){
        //初期化失敗時はゲームを開始しない
        Serial.println("[SYSTEM ERROR] Display initialization failed");
        Serial.println("[SYSTEM ERROR] Game execution stopped");

        // エラー状態として処理を停止
        while(true){
            delay(1000);
        }
    }

    // 表示初期化成功後にゲーム全体を初期化
    initGameManager();
}

void loop(){
    updateGameManager();

    delay(MAIN_LOOP_DELAY_MS);
}