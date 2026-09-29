#include "MiniGame.h"
#include "MiniGameManager.h"
#include "Display.h"

void setup(){
    Serial.begin(115200);

    // ボタン入力ピン設定
    pinMode(2, INPUT_PULLUP);
    pinMode(3, INPUT_PULLUP);
    pinMode(4, INPUT_PULLUP);

    // 乱数ソース
    randomSeed(micros());


    initDisplay();

    startMiniGame(
        selectMiniGame(),
        NORMAL);

    showInstruction(
        getInstruction());

    Serial.println(
        getInstruction());

}

void loop(){
    updateMiniGame();
    if(getCurrentGame() == GAME_JOYSTICK_DOWN){
        showJoystickInfo(getTargetAngle(), getCurrentAngle());
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

    delay(100);
}