/*
ベースミニゲーム
ミニゲームの抽選、値計算、初期化、状態更新
*/

#include <Arduino.h>
#include "GameManager.h"

// ボタン順押し
#define PUSH_ORDER_EASY_COUNT 2
#define PUSH_ORDER_NORMAL_COUNT 3
#define PUSH_ORDER_HARD_COUNT 4

// ジョイスティック中央値
#define JOYSTICK_CENTER_X 512.0f
#define JOYSTICK_CENTER_Y 512.0f

// ジョイスティック不感帯 
#define JOYSTICK_DEADZONE 510

// ジョイスティック回転角
#define FULL_CIRCLE_DEGREES 360.0f
#define HALF_CIRCLE_DEGREES 180.0f

// ジョイスティック角度範囲指定
#define TARGET_ANGLE_STEP 10
#define TARGET_ANGLE_COUNT 36

// ジョイスティック難易度ごとの角度許容範囲
#define EASY_ANGLE_TOLERANCE 45
#define NORMAL_ANGLE_TOLERANCE 25
#define HARD_ANGLE_TOLERANCE 10

// ジョイスティック難易度ごとの回転数
#define EASY_ROTATE_COUNT 3
#define NORMAL_ROTATE_COUNT 5
#define HARD_ROTATE_COUNT 7

// 回転最大割合(100.0%)
#define ROTATE_PERCENT_MAX 100.0f

// 測距パラメータ
#define DISTANCE_TOLERANCE_CM 3
#define DISTANCE_TARGET_MIN_CM 10
#define DISTANCE_TARGET_MAX_CM 40

// 測距ゲーム保持時間
#define EASY_HOLD_TIME    1000
#define NORMAL_HOLD_TIME  2000
#define HARD_HOLD_TIME    2500

// 測距計測パラメータ
#define DISTANCE_ECHO_TIMEOUT_US 30000UL
#define DISTANCE_CONVERSION_VALUE 58.0f


// ミニゲームが成功したかを、複数のソースファイルから参照できるように宣言する
extern bool successFlag;

// ミニゲームが失敗したかを、複数のソースファイルから参照できるように宣言する
extern bool failedFlag;

// 制限時間を超えたかを、複数のソースファイルから参照できるように宣言する
extern bool timeUpFlag;

// ジョイスティックの判定が完了したかを、複数のソースファイルから参照できるように宣言する
extern bool joystickJudged;


// ミニゲーム抽選
MiniGameType selectMiniGame();

// ジョイスティック角度取得
float getJoystickAngle();

// ジョイスティック傾き量判定
bool isJoystickTilted();

// ジョイスティック回転割合取得
int getRotatePercent();

// 指定ボタン押下ゲーム
// 初期化
void initPushGame();
// 状態更新
void updatePushGame();

// ボタン指定順押下ゲーム
// 初期化
void initPushInOrderGame();
// 状態更新
void updatePushInOrderGame();

// ジョイスティック傾倒ゲーム
// 初期化
void initJoystickDownGame();
// 状態更新
void updateJoystickDownGame();

// ジョイスティック回転ゲーム
// 初期化
void initJoystickRotateGame();
// 状態更新
void updateJoystickRotateGame();

// 距離維持ゲーム
// 初期化
void initDistanceGame();
// 状態更新
void updateDistanceGame();
