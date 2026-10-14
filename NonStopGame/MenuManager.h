#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include "MiniGame.h"

// メニュー機能を初期化する
void initMenu(void);

// メニューの入力処理と表示処理を更新する
void updateMenu(void);

// ゲーム終了後などにメニュー状態を初期化する
void resetMenu(void);

// 選択された難易度を取得する
Difficulty getMenuDifficulty(void);

// 選択されたゲーム数を取得する
int getMenuGameCount(void);

// 選択されたゲームスピードを取得する
GameSpeed getMenuSpeed(void);

// スタートが選択されたかを取得する
bool isStartRequested(void);

#endif