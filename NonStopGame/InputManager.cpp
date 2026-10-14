#include <Arduino.h>

#include "InputManager.h"

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

// ========================================
// ボタン状態
// ========================================

// 現在の入力状態
static bool leftPressed = false;
static bool centerPressed = false;
static bool rightPressed = false;

// 前回の入力状態
static bool previousLeftPressed = false;
static bool previousCenterPressed = false;
static bool previousRightPressed = false;

// 押下開始状態
static bool leftTriggered = false;
static bool centerTriggered = false;
static bool rightTriggered = false;

// ========================================
// ジョイスティック状態
// ========================================

static int joystickX = 0;
static int joystickY = 0;

// ========================================
// 測距センサ状態
// ========================================

static float currentDistanceCm =
    DISTANCE_ERROR_VALUE;

static unsigned long lastDistanceUpdateTime = 0;

// ========================================
// 測距センサ内部処理
// ========================================

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

// ========================================
// 初期化
// ========================================

void initInputManager(void)
{
    // ボタン入力ピンを内部プルアップ付き入力に設定する
    pinMode(
        BUTTON_LEFT_PIN,
        INPUT_PULLUP);

    pinMode(
        BUTTON_CENTER_PIN,
        INPUT_PULLUP);

    pinMode(
        BUTTON_RIGHT_PIN,
        INPUT_PULLUP);

    // 測距センサのピン方向を設定する
    pinMode(
        DISTANCE_TRIGGER_PIN,
        OUTPUT);

    pinMode(
        DISTANCE_ECHO_PIN,
        INPUT);

    // 初期ボタン状態を取得する
    leftPressed =
        digitalRead(BUTTON_LEFT_PIN) == LOW;

    centerPressed =
        digitalRead(BUTTON_CENTER_PIN) == LOW;

    rightPressed =
        digitalRead(BUTTON_RIGHT_PIN) == LOW;

    // 初期状態での誤操作を防ぐため、
    // 現在値を前回値へ保存する
    previousLeftPressed =
        leftPressed;

    previousCenterPressed =
        centerPressed;

    previousRightPressed =
        rightPressed;

    // ジョイスティック初期値を取得する
    joystickX = analogRead(JOYSTICK_X_PIN);

    joystickY = analogRead(JOYSTICK_Y_PIN);

    // 最初の測距を実行する
    currentDistanceCm = readDistanceSensor();

    lastDistanceUpdateTime = millis();
}

// ========================================
// 入力更新
// ========================================

void updateInputManager(void){
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

    // センサへの過度な連続アクセスを防ぐ
    if(currentTime - lastDistanceUpdateTime >= DISTANCE_UPDATE_MS){
        currentDistanceCm = readDistanceSensor();

        lastDistanceUpdateTime = currentTime;
    }
}

// ========================================
// ボタン状態取得
// ========================================
bool isLeftButtonPressed(void){
    return leftPressed;
}

bool isCenterButtonPressed(void){
    return centerPressed;
}

bool isRightButtonPressed(void){
    return rightPressed;
}

bool isLeftButtonTriggered(void){
    return leftTriggered;
}

bool isCenterButtonTriggered(void){
    return centerTriggered;
}

bool isRightButtonTriggered(void){
    return rightTriggered;
}

// ========================================
// ジョイスティック状態取得
// ========================================
int getJoystickX(void){
    return joystickX;
}

int getJoystickY(void){
    return joystickY;
}

// ========================================
// 測距値取得
// ========================================
float getDistanceCm(void){
    return currentDistanceCm;
}