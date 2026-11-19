#include <Arduino.h>

#include "MenuManager.h"
#include "Display.h"
#include "InputManager.h"

// // ボタンピン
// #define BUTTON_LEFT_PIN       2
// #define BUTTON_CENTER_PIN     3
// #define BUTTON_RIGHT_PIN      4

// 設定項目数
#define DIFFICULTY_OPTION_COUNT  3
#define SPEED_OPTION_COUNT       3

// ゲーム数設定
#define GAME_COUNT_MIN       3
#define GAME_COUNT_MAX      10
#define GAME_COUNT_DEFAULT   5

// メニュー内部状態
enum MenuState{
    MENU_MAIN,           // メニュー画面
    MENU_DIFFICULTY,     // 難易度選択画面
    MENU_GAME_COUNT,     // ゲーム数選択画面
    MENU_SPEED           // スピード選択画面
};

// メニュー選択項目
enum MainMenuItem{
    MENU_ITEM_EDIT,      // へんしゅう
    MENU_ITEM_START      // スタート
};

// ========================================
// 管理変数
// ========================================
// 現在表示しているメニュー状態
static MenuState currentMenuState = MENU_MAIN;

// メニュー画面のカーソル位置
static int menuIndex = MENU_ITEM_EDIT;

// 選択中の難易度
static Difficulty selectedDifficulty = NORMAL;

// 選択中のゲーム数
static int selectedGameCount = GAME_COUNT_DEFAULT;

// 選択中のスピード
// 初期値はNORMAL
static GameSpeed selectedSpeed = SPEED_NORMAL;

// GameManagerへ通知する開始要求
static bool startRequested = false;

// 前回のボタン状態
// // 押した瞬間だけを検出するために使用する
// static bool previousLeftPressed = false;
// static bool previousCenterPressed = false;
// static bool previousRightPressed = false;

// ========================================
// 初期化
// ========================================

void initMenu(void){
    // 起動時はメニュー画面を表示
    currentMenuState = MENU_MAIN;

    // 初期カーソルは「へんしゅう」
    menuIndex = MENU_ITEM_EDIT;

    // 開始要求を解除する
    startRequested = false;

    // ボタンの前回状態を初期化
    previousLeftPressed = false;
    previousCenterPressed = false;
    previousRightPressed = false;
}

// ========================================
// メニュー更新
// ========================================

void updateMenu(void){
    // InputManagerから押下開始情報を取得する
    bool leftTriggered =
        isLeftButtonTriggered();

    bool centerTriggered =
        isCenterButtonTriggered();

    bool rightTriggered =
        isRightButtonTriggered();

    // 現在のメニュー状態に応じて処理を行う
    switch(currentMenuState){
        // ========================================
        // メニュー画面
        // ========================================

        case MENU_MAIN:{
            // 左右ボタンで
            // 「へんしゅう」と「スタート」を切り替える
            if(leftTriggered || rightTriggered){
                if(menuIndex == MENU_ITEM_EDIT){
                    menuIndex = MENU_ITEM_START;
                }else{
                    menuIndex = MENU_ITEM_EDIT;
                }
            }

            // 決定ボタン押下時
            if(centerTriggered){
                // 「へんしゅう」が選択されている場合
                if(menuIndex == MENU_ITEM_EDIT){
                    currentMenuState = MENU_DIFFICULTY;
                }else{// 「スタート」が選択されている場合
                    startRequested = true;
                }
            }

            // メニュー画面の表示を要求する
            showMainMenu(menuIndex);

            break;
        }

        // ========================================
        // 難易度選択画面
        // ========================================
        case MENU_DIFFICULTY:{
            // 左ボタンで1つ前の難易度へ移動する
            if(leftTriggered){
                selectedDifficulty = (Difficulty)((selectedDifficulty + DIFFICULTY_OPTION_COUNT - 1) % DIFFICULTY_OPTION_COUNT);
            }

            // 右ボタンで1つ次の難易度へ移動する
            if(rightTriggered){
                selectedDifficulty = (Difficulty)((selectedDifficulty + 1) % DIFFICULTY_OPTION_COUNT);
            }

            // 決定ボタンでゲーム数選択へ進む
            if(centerTriggered){
                currentMenuState = MENU_GAME_COUNT;
            }

            // 難易度選択画面の表示を要求する
            showDifficultyMenu(selectedDifficulty);

            break;
        }

        // ========================================
        // ゲーム数選択画面
        // ========================================
        case MENU_GAME_COUNT:{

            // 左ボタンでゲーム数を減らす
            if(leftTriggered){
                // 最小値より大きい場合のみ減らす
                if(selectedGameCount > GAME_COUNT_MIN){
                    selectedGameCount--;
                }
            }

            // 右ボタンでゲーム数を増やす
            if(rightTriggered){
                // 最大値を超えた場合は最大値に戻す
                if(selectedGameCount < GAME_COUNT_MAX){
                    selectedGameCount++;
                }
            }

            // 決定ボタンでスピード選択へ進む
            if(centerTriggered){
                currentMenuState = MENU_SPEED;
            }

            // ゲーム数選択画面の表示を要求する
            showGameCountMenu(selectedGameCount);

            break;
        }

        // ========================================
        // スピード選択画面
        // ========================================

        case MENU_SPEED:
        {
            // 左ボタンで1つ前のスピードへ移動する
            if(leftTriggered){
                // 1つ前のスピードへ切り替える
                selectedSpeed = (GameSpeed)((selectedSpeed + SPEED_OPTION_COUNT - 1) % SPEED_OPTION_COUNT);
            }

            // 右ボタンで1つ次のスピードへ移動する
            if(rightTriggered){
                // 1つ次のスピードへ切り替える
                selectedSpeed =(GameSpeed) ((selectedSpeed + 1) % SPEED_OPTION_COUNT);
            }

            // 決定ボタンで設定を確定し、
            // メニュー画面へ戻る
            if(centerTriggered){
                currentMenuState = MENU_MAIN;

                menuIndex = MENU_ITEM_START;
            }

            // スピード選択画面の表示を要求する
            showSpeedMenu(selectedSpeed);

            break;
        }
    }

    // // 今回のボタン状態を前回値として保存する
    // previousLeftPressed = leftPressed;
    // previousCenterPressed = centerPressed;
    // previousRightPressed = rightPressed;
}

// ========================================
// メニュー状態のリセット
// ========================================
void resetMenu(void){
    // ゲーム終了後はメニュー画面へ戻す
    currentMenuState = MENU_MAIN;

    // メニューのカーソルは「へんしゅう」に戻す
    menuIndex = MENU_ITEM_EDIT;

    // 開始要求を解除する
    startRequested = false;

    // 押しっぱなし状態を引き継がないように
    // 現在のボタン状態を保存する
    previousLeftPressed = digitalRead(BUTTON_LEFT_PIN) == LOW;
    previousCenterPressed = digitalRead(BUTTON_CENTER_PIN) == LOW;
    previousRightPressed = digitalRead(BUTTON_RIGHT_PIN) == LOW;
}

// ========================================
// 設定値取得
// ========================================
Difficulty getMenuDifficulty(void){
    return selectedDifficulty;
}

int getMenuGameCount(void){
    return selectedGameCount;
}

GameSpeed getMenuSpeed(void){
    return selectedSpeed;
}

bool isStartRequested(void){
    return startRequested;
}