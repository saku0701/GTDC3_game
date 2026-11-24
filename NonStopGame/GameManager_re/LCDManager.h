/*
LCDディスプレイマネージャー
LCDディスプレイへの表示関数を持つ
*/
#include "DisplayManager.h"

// 現在のゲーム数、目標ゲーム数、残り時間を表示
void showGameStatus(int currentGame, int totalGame, unsigned long elapsed, unsigned long limit);

// ステータスのリセット
void resetGameStatusDisplay();