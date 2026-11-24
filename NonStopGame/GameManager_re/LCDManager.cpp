// #include <Wire.h>
// #include <LiquidCrystal_I2C.h>

#include "DisplayManager.h"
#include "LCDManager.h"




// ========================================
// LCDの前回表示値
// ========================================
// 前回表示した現在ゲーム番号
static int previousCurrentGame = -1;

// 前回表示した全ゲーム数
static int previousTotalGame = -1;

// 前回表示した残り時間バーの長さ
static int previousRemainingBar = -1;




// 現在のゲーム数、目標ゲーム数、残り時間を表示
void showGameStatus(int currentGame, int totalGame, unsigned long elapsed, unsigned long limit){
    
    // 制限時間が0の場合、0除算を防止する
    if (limit == 0)
    {
        return;
    }

    // 残り時間の割合を計算する
    float remainingRatio = 1.0f - ((float)elapsed / (float)limit);

    // 残り時間が0%未満にならないように補正する
    if(remainingRatio < 0.0f){
        remainingRatio = 0.0f;
    }

    // 残り時間が100%を超えないように補正する
    if(remainingRatio > 1.0f){
        remainingRatio = 1.0f;
    }

    // LCDの横幅16文字に合わせてバーの長さを計算する
    int remainingBar = (int)(remainingRatio * 16.0f);

    // ゲーム数表示に変更があった場合だけ1行目を更新する
    if(currentGame != previousCurrentGame || totalGame != previousTotalGame){
        // 1行目を16文字の空白で上書きする
        lcd.setCursor(0, 0);
        lcd.print("                ");

        // カーソルを1行目の先頭へ戻す
        lcd.setCursor(0, 0);

        // 現在ゲーム数 / 全ゲーム数を表示する
        lcd.print(currentGame);
        lcd.print("/");
        lcd.print(totalGame);

        // 今回の値を前回値として保存する
        previousCurrentGame = currentGame;
        previousTotalGame = totalGame;
    }

    // 残り時間バーに変更があった場合だけ2行目を更新する
    if(remainingBar != previousRemainingBar){
        // 2行目を16文字の空白で上書きする
        lcd.setCursor(0, 1);
        lcd.print("                ");

        // カーソルを2行目の先頭へ戻す
        lcd.setCursor(0, 1);

        // 残り時間分だけバーを表示する
        for(int i = 0; i < remainingBar; i++){
            lcd.print("#");
        }

        // 今回の値を前回値として保存する
        previousRemainingBar = remainingBar;
    }
    
}


// ステータスのリセット
void resetGameStatusDisplay(){
    // 次回showGameStatus()呼び出し時に必ず全項目が更新されるようにする
    previousCurrentGame = -1;
    previousTotalGame = -1;
    previousRemainingBar = -1;

    // LCDの表示を一度だけ消去する
    lcd.clear();
}