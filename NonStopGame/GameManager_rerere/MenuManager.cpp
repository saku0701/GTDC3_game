/*
メニューマネージャー
メニュー状態の管理
*/

#include "MenuManager.h"

// 現在表示しているメニュー状態
extern MenuState currentMenuState = MENU_MAIN;

// メニュー画面のカーソル位置
extern int menuIndex = MENU_ITEM_EDIT;

// 選択中の難易度
exterm Difficulty selectedDifficulty = NORMAL;

// 選択中のゲーム数
extern int selectedGameCount = GAME_COUNT_DEFAULT;

// 選択中のスピード
// 初期値はNORMAL
extern GameSpeed selectedSpeed = SPEED_NORMAL;

// GameManagerへ通知する開始要求
extern bool startRequested = false;

extern previousLeftPressed;
extern previousCenterPressed;
extern previousRightPressed;

// メニュー画面の初期化
void initMenu(void){
    // 起動時はメニュー画面を表示
    currentMenuState = MENU_MAIN;

    // 初期カーソルは編集に
    menuIndex = MENU_ITEM_EDIT;

    // 開始要求を解除
    startRequested = false;

    // ボタンの前回状態を初期化
    previousLeftPressed = false;
    previousCenterPressed = false;
    previousRightPressed = false;
}

// メニュー画面の表示内容更新
void updateMenu(){
    switch (currentMenuState)
    {
        case MENU_MAIN:
            mainMenu();
            break;

        case MENU_DIFFICULTY:
            editGameDifficulty();
            break;

        case MENU_GAME_COUNT:
            editGameCount();
            break;

        case MENU_SPEED:
            editGameSpeed();
            break;        
    }
}

// メインメニュー画面の表示
void mainMenu(){
    // 左右ボタンでカーソル位置を切り替え
    if(leftTriggered || rightTriggered){
        if(menuIndex == MENU_ITEM_EDIT){
            menuIndex = MENU_ITEM_START;
        }else{
            menuIndex = MENU_ITEM_EDIT;
        }
    }

    // 決定ボタン押下時
    if(centerTriggered){
        // 編集項目が選択されている場合
        if(menuIndex == MENU_ITEM_EDIT){
            currentMenuState = MENU_DIFFICULTY;
        }else{// スタートが選択されている場合
            startRequested = true;
        }
    }
}

// 難易度設定画面
void editGameDifficulty(){
    // 左ボタンで1つ前の難易度へ移動
    if(leftTriggered){
        selectedDifficulty = (Difficulty)((selectedDifficulty + DIFFICULTY_OPTION_COUNT - 1) % DIFFICULTY_OPTION_COUNT);
    }

    // 右ボタンで1つ次の難易度へ移動
    if(rightTriggered){
        selectedDifficulty = (Difficulty)((selectedDifficulty + 1) % DIFFICULTY_OPTION_COUNT);
    }

    // 決定ボタンでゲーム数選択へ進む
    if(centerTriggered){
        currentMenuState = MENU_GAME_COUNT;
    }
}

// ゲーム数設定画面
void editGameCount(){
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
}

// ゲームスピード設定画面
void editGameSpeed(){
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
}
