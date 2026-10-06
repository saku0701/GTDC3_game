#include "MiniGame.h"
#include "MiniGameManager.h"
#include "Display.h"
#include "GameManager.h"

void setup(){
    Serial.begin(115200);

    // ボタン入力ピン設定
    pinMode(2, INPUT_PULLUP);
    pinMode(3, INPUT_PULLUP);
    pinMode(4, INPUT_PULLUP);

    // HC-SR04設定
    pinMode(9, OUTPUT);  // TRIG
    pinMode(8, INPUT);   // ECHO

    // 乱数初期化
    randomSeed(micros());

    initDisplay();

    initGameManager();

    // あとで消す
    // Serial.println(getInstruction());

}

void loop(){
    updateGameManager();

    delay(10);
}