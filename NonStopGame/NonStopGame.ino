#include "MiniGame.h"
#include "MiniGameManager.h"
#include "Display.h"

void setup(){
    Serial.begin(115200);

    // ボタン入力ピン設定
    pinMode(2, INPUT_PULLUP);
    pinMode(3, INPUT_PULLUP);
    pinMode(4, INPUT_PULLUP);

    // HC-SR04設定
    pinMode(9, OUTPUT);  // TRIG
    pinMode(8, INPUT);   // ECHO

    // 乱数ソース
    randomSeed(micros());


    initDisplay();

    startMiniGame( selectMiniGame(), NORMAL);

    showInstruction(getInstruction());

    Serial.println(getInstruction());

}

void loop(){
    updateMiniGame();
    
    // ジョイスティックゲームの時
    if(getCurrentGame() == GAME_JOYSTICK_DOWN){
        showJoystickInfo(getTargetAngle(), getCurrentAngle());
    }

    // 測距ゲームの時
    if(getCurrentGame() == GAME_DISTANCE_KEEP){
        showDistanceInfo(getTargetDistance(), getCurrentDistance());
    }

    // ジョイスティック回転ゲームの時
    if(getCurrentGame() == GAME_JOYSTICK_ROTATE){
        showRotateInfo( getTargetRotateCount(), getCompletedRotateCount(), getCurrentRotatePercent());
    }


    //ミニゲーム成功
    if(isMiniGameSuccess()){
        showSuccess();
        Serial.println("SUCCESS");
    }

    //ミニゲーム失敗
    if(isMiniGameFailed()){
        showFailed();
        Serial.println("FAILED");
    }

    if(isTimeUp()){
        if(isMiniGameSuccess()){
            Serial.println("NEXT GAME");
        }
        else{
            Serial.println("GAME OVER");
        }

        while(true);
    }

    delay(10);
}