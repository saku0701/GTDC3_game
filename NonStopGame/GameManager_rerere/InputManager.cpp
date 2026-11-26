/*
メニューマネージャー
メニュー状態の管理
*/

#include "InputManager.h"

extern bool leftPressed;
extern bool centerPressed;
extern bool rightPressed;

extern bool previousLeftPressed;
extern bool previousCenterPressed;
extern bool previousRightPressed;

extern bool leftTriggered;
extern bool centerTriggered;
extern bool rightTriggered;


extern int joystickX;
extern int joystickY;

extern int currentDistanceCm;
extern unsigned long lastDistanceUpdateTime;

// 測距センサの値を取得して変換
static float readDistanceSensor(void)
{
    // 安定したパルスを生成するため、一度LOWにする
    digitalWrite(
        DISTANCE_TRIGGER_PIN,
        LOW);

    delayMicroseconds(
        TRIGGER_LOW_TIME_US);

    // 10マイクロ秒のTRIG信号を送信する
    digitalWrite(
        DISTANCE_TRIGGER_PIN,
        HIGH);

    delayMicroseconds(
        TRIGGER_HIGH_TIME_US);

    digitalWrite(
        DISTANCE_TRIGGER_PIN,
        LOW);

    // ECHO信号のHIGH期間を測定する
    unsigned long duration =
        pulseIn(
            DISTANCE_ECHO_PIN,
            HIGH,
            ECHO_TIMEOUT_US);

    // タイムアウトした場合は代替値を返す
    if(duration == 0)
    {
        return DISTANCE_ERROR_VALUE;
    }

    // パルス時間を距離へ変換する
    return
        (float)duration /
        DISTANCE_DIVISOR;
}

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
