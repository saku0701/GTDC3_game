/*
ゲームマネージャー
ゲーム全体の状態変数はここ
*/

#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

// ゲーム全体の状態
enum GameState {
    MENU_INIT,
    MENU,
    GAME_INIT,
    STATE_COUNTDOWN,
    STATE_Instruction,
    GAME_MINIGAME,
    STATE_SUCCESS,
    STATE_GAMECLEAR,
    STATE_GAMEOVER
};

// メニュー状態
enum MenuState{
    MENU_MAIN,
    MENU_DIFFICULTY,
    MENU_GAME_COUNT,
    MENU_SPEED
};

// ゲーム難易度
enum Difficulty{
    EASY,
    NORMAL,
    HARD
};

// ゲームスピード
// 制限時間を決定するために使用する
enum GameSpeed{
    SPEED_SLOW,    // 制限時間：10秒
    SPEED_NORMAL,  // 制限時間：7秒
    SPEED_FAST     // 制限時間：5秒
};

// ミニゲーム一覧
enum MiniGameType{
    GAME_PUSH,
    GAME_PUSH_INORDER,
    GAME_JOYSTICK_DOWN,
    GAME_DISTANCE_KEEP,
    GAME_JOYSTICK_ROTATE,
    
    GAME_TYPE_COUNT//実装ゲーム数カウント
};

#endif // GAME_MANAGER_H