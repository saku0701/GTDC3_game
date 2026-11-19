#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <Arduino.h>

// ========================================
// InputManager初期化
// ========================================

// 入力ピンと内部状態を初期化する
void initInputManager(void);

// 全入力情報を更新する
void updateInputManager(void);

// ========================================
// ボタン入力取得
// ========================================

// 現在ボタンが押されているか取得する
bool isLeftButtonPressed(void);
bool isCenterButtonPressed(void);
bool isRightButtonPressed(void);

// 今回の更新でボタンが押されたか取得する
// 押しっぱなしではなく押した瞬間だけtrueを返す
bool isLeftButtonTriggered(void);
bool isCenterButtonTriggered(void);
bool isRightButtonTriggered(void);

// ========================================
// ジョイスティック入力取得
// ========================================

// ジョイスティックのX軸・Y軸を取得する
int getJoystickX(void);
int getJoystickY(void);

// ========================================
// 測距センサ入力取得
// ========================================

// 最新の測距値をcm単位で取得する
float getDistanceCm(void);

#endif