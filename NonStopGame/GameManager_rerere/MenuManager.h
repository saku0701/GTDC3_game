/*
メニューマネージャー
メニュー状態の管理
*/

#include <Arduino.h>

#include "DisplayManager.h"
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





// メニュー画面の初期化
void initMenu(void);

// メニュー画面の表示内容更新
void updateMenu();

// メインメニュー画面の表示
void mainMenu();

// 難易度設定画面
void editGameDifficulty();

// ゲーム数設定画面
void editGameCount();

// ゲームスピード設定画面
void editGameSpeed();

