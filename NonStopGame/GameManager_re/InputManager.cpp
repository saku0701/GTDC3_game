/*
メニューマネージャー
メニュー状態の管理
*/

#include "InputManager.h"


// 入力ピン初期化
void initInputManager(){

    // ボタン入力ピンを内部プルアップ付き入力に設定
    pinMode(BUTTON_LEFT_PIN, INPUT_PULLUP);
    pinMode(BUTTON_CENTER_PIN, INPUT_PULLUP);
    pinMode(BUTTON_RIGHT_PIN, INPUT_PULLUP);

    // 測距センサのピン方向を設定
    pinMode(DISTANCE_TRIGGER_PIN, OUTPUT);
    pinMode(DISTANCE_ECHO_PIN, INPUT);

    // 初期ボタン状態を取得する
    leftPressed = digitalRead(BUTTON_LEFT_PIN) == LOW;
    centerPressed = digitalRead(BUTTON_CENTER_PIN) == LOW;
    rightPressed = digitalRead(BUTTON_RIGHT_PIN) == LOW;

    // 初期状態での誤操作防止
    previousLeftPressed = leftPressed;
    previousCenterPressed = centerPressed;
    previousRightPressed = rightPressed;

    // ジョイスティック初期値を取得
    joystickX = analogRead(JOYSTICK_X_PIN);
    joystickY = analogRead(JOYSTICK_Y_PIN);

    // 最初の測距を実行
    currentDistanceCm = readDistanceSensor();
    lastDistanceUpdateTime = millis();
}

// 入力状態更新
void updateInputManager(){
    // ----------------------------------------
    // ボタン更新
    // ----------------------------------------
    // 前回値を保存する
    previousLeftPressed = leftPressed;
    previousCenterPressed = centerPressed;
    previousRightPressed = rightPressed;

    // 現在値を取得する
    leftPressed = digitalRead(BUTTON_LEFT_PIN) == LOW;
    centerPressed = digitalRead(BUTTON_CENTER_PIN) == LOW;
    rightPressed = digitalRead(BUTTON_RIGHT_PIN) == LOW;

    // 押されていない状態から押された状態への変化を検出する
    leftTriggered = leftPressed && !previousLeftPressed;
    centerTriggered = centerPressed && !previousCenterPressed;
    rightTriggered = rightPressed && !previousRightPressed;

    // ----------------------------------------
    // ジョイスティック更新
    // ----------------------------------------
    joystickX = analogRead(JOYSTICK_X_PIN);
    joystickY = analogRead(JOYSTICK_Y_PIN);

    // ----------------------------------------
    // 測距センサ更新
    // ----------------------------------------
    unsigned long currentTime = millis();
    // センサへの過度な連続アクセス防止
    if(currentTime - lastDistanceUpdateTime >= DISTANCE_UPDATE_MS){
        currentDistanceCm = readDistanceSensor();
        lastDistanceUpdateTime = currentTime;
    }
}

// 左ボタン状態取得
bool isLeftButtonPressed(void){
    return leftPressed;
}
// 中央ボタン状態取得
bool isCenterButtonPressed(void){
    return centerPressed;
}
// 右ボタン状態取得
bool isRightButtonPressed(void){
    return rightPressed;
}
// ジョイスティックのX座標の値取得
int getJoystickX(void){
    return joystickX;
}
// ジョイスティックのY座標の値取得
int getJoystickY(void){
    return joystickY;
}
// 測距センサ入力取得
float getDistanceCm(void){
    return currentDistanceCm;
}
