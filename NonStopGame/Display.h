#include "MiniGame.h"
#ifndef DISPLAY_H
#define DISPLAY_H

// OLEDとLCDを初期化
// 成功時はtrue、失敗時はfalseを返す
bool initDisplay(void);

// ミニゲーム指示表示
void showInstruction(const char* text);

// ミニゲーム成功画面表示
void showSuccess();

// ミニゲーム失敗画面
void showFailed();

// ジョイスティック表示
void showJoystickInfo(int targetAngle, int currentAngle);

// 測距表示
void showDistanceInfo(int targetDistance, int currentDistance);

// ジョイスティック回転表示
void showRotateInfo(int targetCount, int completedCount, int percent);

// メニュー表示
void showMenuScreen(Difficulty difficulty);

// カウントダウン表示
void showCountdown(int count);

// ゲームクリア表示
void showGameClear();

// ゲームステータス表示
void showGameStatus(int currentGame, int totalGame, unsigned long elapsed, unsigned long limit);

// ゲームステータス表示のリセット
void resetGameStatusDisplay();

// メニュー画面を表示する
void showMainMenu(int menuIndex);

// 難易度選択画面を表示する
void showDifficultyMenu(Difficulty difficulty);

// ゲーム数選択画面を表示する
void showGameCountMenu(int gameCount);

// スピード選択画面を表示する
void showSpeedMenu(int speedIndex);

#endif