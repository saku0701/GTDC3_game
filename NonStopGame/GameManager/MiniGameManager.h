/*
ミニゲームマネージャー
ミニゲームの初期化、状態更新
*/

#include "GameManager.h"

// スピードごとの制限時間
#define SLOW_TIME_LIMIT    (10UL * 1000UL)
#define NORMAL_TIME_LIMIT   (7UL * 1000UL)
#define FAST_TIME_LIMIT     (3UL * 1000UL)

// ミニゲーム開始処理
void startMiniGame(MiniGameType gameType, Difficulty dificulty, GameSpeed speed);

// ミニゲーム状態更新
void updateMiniGame();