#ifndef MINIGAME_H
#define MINIGAME_H

// ゲーム難易度
enum Difficulty
{
    EASY,
    NORMAL,
    HARD
};

/* 制限時間(ms) */
#define EASY_TIME_LIMIT 5000
#define NORMAL_TIME_LIMIT 3000
#define HARD_TIME_LIMIT 1500

// ミニゲーム一覧
enum MiniGameType{
    GAME_PUSH,
    GAME_PUSH_INORDER,
    GAME_JOYSTICK_DOWN,
    GAME_DISTANCE_KEEP,
    GAME_JOYSTICK_ROTATE
};

// ミニゲーム開始
void startMiniGame(
    MiniGameType gameType,
    Difficulty difficulty);

// ゲーム状態更新
void updateMiniGame(void);

// ミニゲーム成功時
bool isMiniGameSuccess(void);

// ミニゲーム失敗時
bool isMiniGameFailed(void);

// 時間制限
bool isTimeUp(void);

// 角度
int getTargetAngle(void);
int getCurrentAngle(void);
MiniGameType getCurrentGame(void);

// 測距
int getTargetDistance(void);
int getCurrentDistance(void);

// ジョイスティック回転
int getTargetRotateCount(void);
int getCompletedRotateCount(void);
int getCurrentRotatePercent(void);

// ミニゲーム指示表示
const char* getInstruction(void);

#endif