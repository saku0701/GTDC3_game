/*
ミニゲームマネージャー
ミニゲームの初期化、状態更新
*/

#include <Arduino.h>          
#include "MiniGameManager.h"  
#include "BaseMiniGame.h"     

// 現在実行しているミニゲームを保持する
extern MiniGameType currentGame;

// 現在設定されている難易度を保持する
extern Difficulty currentDifficulty;

// 現在設定されているゲーム速度を保持する
extern GameSpeed currentSpeed;

// ミニゲームを開始した時刻をミリ秒単位で保持する
extern unsigned long startTime = 0;

// ミニゲームの制限時間をミリ秒単位で保持する
extern unsigned long timeLimit = 0;

extern MiniGameType currentGame;


// ミニゲーム開始処理
void startMiniGame(MiniGameType gameType, Difficulty dificulty, GameSpeed speed){
    // 実行するミニゲームを抽選
    currentGame = selectMiniGame();

    // ゲームクリア判定フラグ
    successFlag = false;
    failedFlag = false;
    timeUpFlag = false;
    joystickJudged = false;

    // ゲーム開始時間取得
    startTime = millis();

    // スピード設定に応じて制限時間を決定する
    switch(currentSpeed){
        case SPEED_SLOW:
            // SLOWは制限時間10秒
            timeLimit = SLOW_TIME_LIMIT;
            break;

        case SPEED_NORMAL:
            // NORMALは制限時間7秒
            timeLimit = NORMAL_TIME_LIMIT;
            break;

        case SPEED_FAST:
            // FASTは制限時間3秒
            timeLimit = FAST_TIME_LIMIT;
            break;

        default:
            // 不正な設定値の場合はNORMALを代替値とする
            currentSpeed = SPEED_NORMAL;
            timeLimit = NORMAL_TIME_LIMIT;
            break;
    }

    // デバッグ用：開始時の設定値をシリアルモニタへ表示する
    // Serial.print("Difficulty=");
    // Serial.print((int)currentDifficulty);

    // Serial.print(" Speed=");
    // Serial.print((int)currentSpeed);

    // Serial.print(" TimeLimit=");
    // Serial.println((int)timeLimit);


    switch(currentGame){
        case GAME_PUSH:
            initPushGame();
            break;

        case GAME_PUSH_INORDER:
            initPushInOrderGame();
            break;

        case GAME_JOYSTICK_DOWN:
            initJoystickDownGame();
            break;

        case GAME_DISTANCE_KEEP:
            initDistanceGame();
            break;

        case GAME_JOYSTICK_ROTATE:
            initJoystickRotateGame();
            break;
    }
}

// ミニゲーム状態更新
void updateMiniGame(){
    /* 制限時間監視 */
    if(millis() - startTime >= timeLimit){
        timeUpFlag = true;
        return;
    }

    switch(currentGame){
        case GAME_PUSH://ボタン押下ゲーム
            updatePushGame();
            break;
        case GAME_PUSH_INORDER://ボタン順押下ゲーム
            updatePushInOrderGame();
            break;
        case GAME_JOYSTICK_DOWN://ジョイスティック傾倒ゲーム
            updateJoystickDownGame();
            break;
        case GAME_DISTANCE_KEEP://距離維持ゲーム
            updatePushGame();
            break;
        case GAME_JOYSTICK_ROTATE://ボタン押しゲーム
            updatePushGame();
            break;
    }
}