/*
OLEDディスプレイマネージャー
OLEDディスプレイへの表示関数を持つ
*/

#include "GameManager.h"
#include "DisplayManager.h"

// メニュー表示
void showMenuScreen(Difficulty difficulty);

// 難易度選択画面を表示する
void showDifficultyMenu(Difficulty difficulty);

// ゲーム数選択画面を表示する
void showGameCountMenu(int gameCount);

// スピード選択画面を表示する
void showSpeedMenu(int speedIndex);

// カウントダウン表示
void showCountdown(int count);

// ミニゲーム指示表示
void showInstruction(const char* text);

// ミニゲーム成功画面表示
void showSuccess();

// ゲームクリア表示
void showGameClear();

// ミニゲーム失敗画面表示
void showFailed();

// ボタン押下ゲーム画面表示
void showPushGame(ButtonType targetButton, Difficulty diffibulty);

// ボタン指定順押下ゲーム画面表示
void showPushInOrderGame(ButtonType *targetButton, Difficulty diffibulty);

// ジョイスティック傾倒ゲーム表示
void showJoystickDownGame(int targetAngle, int currentAngle);

// ジョイスティック回転表示
void showRotateInfo(int targetCount, int completedCount, int percent);

// 距離維持ゲーム画面表示
void showDistanceGame(int targetDistance, int currentDistance);

#endif