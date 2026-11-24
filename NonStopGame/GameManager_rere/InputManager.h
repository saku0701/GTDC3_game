/*
インプットマネージャー
入力状態を取得する
*/

#include <Arduino.h>

// ========================================
// 入力ピン設定
// ========================================

#define BUTTON_LEFT_PIN       2
#define BUTTON_CENTER_PIN     3
#define BUTTON_RIGHT_PIN      4

#define JOYSTICK_X_PIN       A0
#define JOYSTICK_Y_PIN       A1

#define DISTANCE_ECHO_PIN     8
#define DISTANCE_TRIGGER_PIN  9


// ========================================
// 測距センサ設定
// ========================================

// TRIG信号をLOWにする時間
#define TRIGGER_LOW_TIME_US       2

// TRIG信号をHIGHにする時間
#define TRIGGER_HIGH_TIME_US     10

// ECHO待機タイムアウト
#define ECHO_TIMEOUT_US       30000UL

// パルス時間から距離へ変換する値
#define DISTANCE_DIVISOR         58.0f

// 測距センサの更新間隔
// HC-SR04の連続測定を防ぐ
#define DISTANCE_UPDATE_MS       60UL

// 測距失敗時の代替値
#define DISTANCE_ERROR_VALUE      0.0f




// 入力ピン初期化
void initInputManager();

// 入力状態更新
void updateInputManager();

// 左ボタン状態取得
bool isLeftButtonPressed();

// 中央ボタン状態取得
bool isCenterButtonPressed();

// 右ボタン状態取得
bool isRightButtonPressed();

// ジョイスティックのX座標の値取得
int getJoystickX();

// ジョイスティックのY座標の値取得
int getJoystickY();

// 測距センサ入力取得
float getDistance();

