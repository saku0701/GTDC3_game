/*
ゲームマネージャー
ゲーム全体の状態変数はここ
*/

#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#define PUSH_ORDER_MAX_COUNT 4
#define PUSH_ORDER_EASY_COUNT 2
#define PUSH_ORDER_NORMAL_COUNT 3
#define PUSH_ORDER_HARD_COUNT 4

#define DISTANCE_ERROR_VALUE 0.0f


// ゲーム全体の状態
enum GameState {
    MENU_INIT,
    MENU,
    GAME_INIT,
    STATE_COUNTDOWN,
    STATE_Instruction,
    STATE_MINIGAME,
    STATE_SUCCESS,
    STATE_GAMECLEAR,
    STATE_GAMEOVER
};

// メニュー内部状態
enum MenuState{
    MENU_MAIN,           // メニュー画面
    MENU_DIFFICULTY,     // 難易度選択画面
    MENU_GAME_COUNT,     // ゲーム数選択画面
    MENU_SPEED           // スピード選択画面
};

// メニュー選択項目
enum MainMenuItem{
    MENU_ITEM_EDIT,      // へんしゅう
    MENU_ITEM_START      // スタート
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


// ボタン
enum ButtonType{
    BUTTON_LEFT,
    BUTTON_CENTER,
    BUTTON_RIGHT
};





#endif // GAME_MANAGER_H