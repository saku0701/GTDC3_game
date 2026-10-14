#include <Arduino.h>
#include "GameManager.h"
#include "MenuManager.h"
#include "MiniGame.h"
#include "MiniGameManager.h"
#include "Display.h"
#include "InputManager.h"

// カウントダウン時間
#define COUNTDOWN_DURATION_MS 3000UL

// カウントダウン開始値
#define COUNTDOWN_START_VALUE 3

// ミニゲーム成功画面を表示する時間
#define SUCCESS_DISPLAY_TIME_MS 1000UL

// 1秒を表すミリ秒
#define MILLISECONDS_PER_SECOND 1000UL

// 中央ボタンのピン番号
#define BUTTON_CENTER_PIN 3

// タイマ変数
static unsigned long stateStartTime;

// 状態定義
enum GameState{
    STATE_MENU,
    STATE_COUNTDOWN,
    STATE_MINIGAME,
    STATE_SUCCESS,
    STATE_GAMEOVER,
    STATE_GAMECLEAR
};

// 管理変数
static GameState currentState;

// 成功済みのミニゲーム数
static int gameCount;

// メニューで設定された全ゲーム数
static int targetGameCount;

void initGameManager(){
    currentState = STATE_MENU;

    gameCount = 0;

    // 初期状態ではMenuManagerの設定値を取得する
    targetGameCount = getMenuGameCount();

    stateStartTime = millis();

    // MenuManager自体も初期化する
    initMenu();
}

void updateGameManager(){
    switch(currentState){
        case STATE_MENU:
        // メニュー入力処理と画面描画はMenuManagerへまとめて任せる
        updateMenu();

        if(isStartRequested()){
            // 成功済みゲーム数を初期化する
            gameCount = 0;

            // メニューで選択した総ゲーム数を取得する
            targetGameCount = getMenuGameCount();

            // LCDの前回表示値を初期化する
            resetGameStatusDisplay();
            Serial.println("GAME START");

            // カウントダウン状態へ遷移する
            currentState = STATE_COUNTDOWN;

            // カウントダウン開始時刻を保存する
            stateStartTime = millis();
        }

        break;

        case STATE_COUNTDOWN:{
            // カウントダウン開始からの経過時間を取得する
            unsigned long elapsed = millis() - stateStartTime;

            // 3秒経過した場合はミニゲームへ遷移する
            if(elapsed >= COUNTDOWN_DURATION_MS){
                // 抽選したミニゲームを設定された難易度で開始する
                startMiniGame(selectMiniGame(), getMenuDifficulty(), getMenuSpeed());

                // ミニゲーム状態へ遷移する
                currentState = STATE_MINIGAME;

                // このループではカウントダウン画面を再描画しない
                break;
            }

            // 経過時間に応じて3、2、1を計算する
            int remain = COUNTDOWN_START_VALUE - (elapsed / MILLISECONDS_PER_SECOND);

            // カウントダウン画面を表示する
            showCountdown(remain);

            break;
        }

        case STATE_MINIGAME:
          updateMiniGame();

          if(getCurrentGame() == GAME_PUSH){
              showInstruction(getInstruction());
          }

          if(getCurrentGame() == GAME_PUSH_INORDER){
              showInstruction(getInstruction());
          }

          if(getCurrentGame() == GAME_JOYSTICK_DOWN){
              showJoystickInfo( getTargetAngle(), getCurrentAngle());
          }

          if(getCurrentGame() == GAME_DISTANCE_KEEP){showDistanceInfo( getTargetDistance(), getCurrentDistance());
          }

          if(getCurrentGame() == GAME_JOYSTICK_ROTATE){
              showRotateInfo(getTargetRotateCount(), getCompletedRotateCount(), getCurrentRotatePercent());
          }

          if(isMiniGameSuccess()){
              currentState = STATE_SUCCESS;
              stateStartTime = millis();
          }

          if(isMiniGameFailed()){
              currentState = STATE_GAMEOVER;
              stateStartTime = millis();
          }

          if(isTimeUp()){
              currentState = STATE_GAMEOVER;
              stateStartTime = millis();
          }

          showGameStatus(
              gameCount + 1,        // 現在挑戦中のゲーム番号(クリア数+1)
              targetGameCount,     // 全ゲーム数
              getElapsedTime(),     // ミニゲーム開始からの経過時間
              getTimeLimit()        // ミニゲームの制限時間
          );
          
          break;

        case STATE_SUCCESS:
          showSuccess();
          if(millis() - stateStartTime >= SUCCESS_DISPLAY_TIME_MS){
              gameCount++;
              Serial.print("gameCount=");
              Serial.println(gameCount);

              if(gameCount >= targetGameCount){
                  currentState = STATE_GAMECLEAR;

                  Serial.println("GAME CLEAR");
              }else{
                  startMiniGame(selectMiniGame(), getMenuDifficulty(), getMenuSpeed());
                  currentState = STATE_MINIGAME;
              }
          }
          break;

        case STATE_GAMEOVER:
          showFailed();
          if(isCenterButtonTriggered()){
            resetMenu();
            currentState = STATE_MENU;

            Serial.println("BACK TO MENU");//デバッグ用文字
          }
          Serial.println("GAME OVER");//デバッグ用失敗文字
          break;

        case STATE_GAMECLEAR:
          showGameClear();
          if(isCenterButtonTriggered()){
            resetMenu();
            currentState = STATE_MENU;
            Serial.println("BACK TO MENU");//デバッグ用文字
          }
          Serial.println("GAME CLEAR");//デバッグ用クリア文字
          break;

        
    }
}