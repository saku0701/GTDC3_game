#include "MiniGame.h"
#ifndef DISPLAY_H
#define DISPLAY_H

// ディスプレイ初期化
void initDisplay();

// ミニゲーム指示表示
void showInstruction(
    const char* text);

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

#endif