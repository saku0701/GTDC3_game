#include "LCDManager.h"
#include "OLEDManager.h"
#include "GameManager.h"

// メニュー表示
void showMenuScreen(int menuIndex){
    oled.clearDisplay();

    oled.setTextColor(WHITE);
    oled.setTextSize(TEXTSIZE_SMALL);

    // タイトル
    oled.setCursor(14, 2);
    oled.println("NON STOP GAME");

    // 「EDIT」表示
    oled.setCursor(10, 25);

    if(menuIndex == 0){
        oled.print("> ");
    }else{
        oled.print("  ");
    }

    oled.println("EDIT");

    // 「START」表示
    oled.setCursor(10, 45);

    if(menuIndex == 1){
        oled.print("> ");
    }else{
        oled.print("  ");
    }

    oled.println("START");
}

// 難易度選択画面を表示する
void showDifficultyMenu(Difficulty difficulty){
    oled.clearDisplay();

    // テキスト設定
    oled.setTextColor(WHITE);
    oled.setTextSize(TEXTSIZE_SMALL);

    // 「SELECT DIFFICULTY」表示
    oled.setCursor(0, 2);
    oled.println("SELECT DIFFICULTY");

    // 選択中の難易度の表示
    oled.setCursor(18, 25);
    oled.setTextSize(TEXTSIZE_MIDDLE);
    switch(difficulty){
        case EASY:
            oled.println("EASY");
            break;

        case NORMAL:
            oled.println("NORMAL");
            break;

        case HARD:
            oled.println("HARD");
            break;
    }

    // 操作説明
    oled.setTextSize(TEXTSIZE_SMALL);
    oled.setCursor(0, 55);
    oled.println("< > SELECT  OK NEXT");

    oled.display();
}

// ゲーム数選択画面を表示する
void showGameCountMenu(int gameCount){
    oled.clearDisplay();

    // テキスト設定
    oled.setTextColor(WHITE);
    oled.setTextSize(TEXTSIZE_SMALL);

    // 「SELECT GAME COUNT」表示
    oled.setCursor(0, 2);
    oled.println("SELECT GAME COUNT");

    // 選択中のゲーム数表示
    oled.setTextSize(TEXTSIZE_BIG);
    oled.setCursor(52, 22);
    oled.println(gameCount);

    // 操作説明
    oled.setTextSize(TEXTSIZE_SMALL);
    oled.setCursor(0, 55);
    oled.println("< > CHANGE  OK NEXT");

    oled.display();
}

// スピード選択画面を表示する
void showSpeedMenu(int speedIndex){
    oled.clearDisplay();

    // テキスト設定
    oled.setTextColor(WHITE);
    oled.setTextSize(TEXTSIZE_SMALL);

    // 「SELECT SPEED」表示
    oled.setCursor(0, 2);
    oled.println("SELECT SPEED");

    // 選択中のスピード表示
    oled.setTextSize(TEXTSIZE_MIDDLE);
    oled.setCursor(10, 25);

    switch(speedIndex){
        case SPEED_SLOW:
            oled.println("SLOW");
            break;

        case SPEED_NORMAL:
            oled.println("NORMAL");
            break;

        case SPEED_FAST:
            oled.println("FAST");
            break;

        default:
            oled.println("NORMAL");
            break;
    }

    // 操作説明
    oled.setTextSize(TEXTSIZE_SMALL);
    oled.setCursor(0, 55);
    oled.println("< > SELECT  OK SET");

    oled.display();
}

// カウントダウン表示
void showCountdown(int count){
    oled.clearDisplay();
    oled.setTextColor(WHITE);
    oled.setTextSize(TEXTSIZE_BIG);
    oled.setCursor(50,20);
    oled.print(count);
    oled.display();
}

// ミニゲーム指示表示
void showInstruction(const char* instruction){
    oled.clearDisplay();
    oled.setTextColor(WHITE);
    oled.setTextSize(TEXTSIZE_MIDDLE);
    oled.setCursor(10, 20);
    oled.println(instruction);
    oled.display();
}

// ミニゲーム成功画面表示
void showSuccess(){
    oled.clearDisplay();
    oled.setTextColor(WHITE);
    oled.setTextSize(TEXTSIZE_BIG);
    oled.setCursor(15,20);
    oled.println("OK!");
    oled.display();
}

// ゲームクリア表示
void showGameClear(){
    oled.clearDisplay();
    oled.setTextColor(WHITE);
    oled.setTextSize(TEXTSIZE_MIDDLE);
    oled.setCursor(0,20);
    oled.println("CLEAR!");
    oled.display();
}

// ミニゲーム失敗画面表示
void showFailed(){
    oled.clearDisplay();
    oled.setTextColor(WHITE);
    oled.setTextSize(TEXTSIZE_BIG);
    oled.setCursor(10,20);
    oled.println("MISS");
    oled.display();
}

// ボタン押下ゲーム画面表示
void showPushGame(ButtonType targetButton, Difficulty difficulty){
    oled.clearDisplay();

    oled.setTextSize(TEXTSIZE_MIDDLE);
    oled.setCursor(20,10);

    switch(difficulty){
        case EASY:
            switch(targetButton)
            {
                case BUTTON_LEFT:
                    // 左ボタン指示
                    oled.println("LEFT");
                    break;

                case BUTTON_CENTER:
                    // 中央ボタン指示
                    oled.println("CENTER");
                    break;

                case BUTTON_RIGHT:
                    // 右ボタン指示
                    oled.println("RIHGT");
                    break;
                
                // default:
                //     // 例外時
                //     oled.println(instruction, "CENTER");
                //     break;
            }
            break;

        case NORMAL:
            switch(targetButton)
            {
                case BUTTON_LEFT:
                    // 左ボタン指示
                    oled.println("GREEN");
                    break;

                case BUTTON_CENTER:
                    // 中央ボタン指示
                    oled.println("YELLOW");
                    break;

                case BUTTON_RIGHT:
                    // 右ボタン指示
                    oled.println("RED");
                    break;
                
                // default:
                //     // 例外時
                //     oled.println(instruction, "YELLOW");
                //     break;
            }
            break;

        case HARD:
            switch(targetButton)
            {
                case BUTTON_LEFT:
                    // 左ボタン指示
                    oled.println("LEFT  of YELLOW");
                    break;

                case BUTTON_CENTER:
                    // 中央ボタン指示
                    oled.println("RIGHT of GREEN");
                    break;

                case BUTTON_RIGHT:
                    // 右ボタン指示
                    oled.println("RIGHT of YELLOW");
                    break;
                
                // default:
                //     // 例外時
                //     oled.println(instruction, "RIGHT of GREEN");
                //     break;
            }
            break;
    }

    oled.display();
    // Serial.print("Instruction=");
    // Serial.println(instruction);
}

// ボタン指定順押下ゲーム画面表示
void showPushInOrderGame(ButtonType targetButtonInOrder[], Difficulty difficulty, char instruction[]){
    int targetCount = 3;
    oled.clearDisplay();

    oled.setTextSize(TEXTSIZE_MIDDLE);
    oled.setCursor(20,10);

    switch(difficulty){
        case EASY:
            // 2
            targetCount = PUSH_ORDER_EASY_COUNT;
            break;

        case NORMAL:
            // 3
            targetCount = PUSH_ORDER_NORMAL_COUNT;
            break;

        case HARD:
            // 4
            targetCount = PUSH_ORDER_HARD_COUNT;
            break;
    }

    instruction[0] = '\0';

    for(int i = 0; i < targetCount; i++){
        targetButtonInOrder[i] = (ButtonType)random(0, 3);

        switch(targetButtonInOrder[i]){
            case BUTTON_LEFT:
                strcat(instruction, "L");
                break;

            case BUTTON_CENTER:
                strcat(instruction, "C");
                break;

            case BUTTON_RIGHT:
                strcat(instruction, "R");
                break;
        }

        if(i != targetCount - 1){
            strcat(instruction, "->");
        }
    }

    oled.println(instruction);
    oled.display();
}

// ジョイスティック傾倒ゲーム表示
void showJoystickDownGame(int targetAngle, int currentAngle){
    oled.clearDisplay();

    // テキスト設定
    oled.setTextColor(WHITE);
    oled.setTextSize(TEXTSIZE_SMALL);

    // 目標角度
    oled.setCursor(0, 0);
    oled.print("TARGET");
    oled.setCursor(0, 10);
    oled.print(targetAngle);
    oled.print(" DEG");

    // 現在角度
    oled.setCursor(0, 35);
    oled.print("NOW");
    oled.setCursor(0, 45);
    oled.print(currentAngle);
    oled.print(" DEG");

    oled.display();
}

// ジョイスティック回転表示
void showJoystickRotateGame(int targetCount, int completedCount, int percent){
    oled.clearDisplay();

    // テキスト設定
    oled.setTextSize(TEXTSIZE_SMALL);

    // 目標回転数
    oled.setCursor(0,0);
    oled.print("TARGET:");
    oled.println(targetCount);

    // 完了回転数
    oled.setCursor(0,20);
    oled.print("DONE:");
    oled.println(completedCount);

    // 進捗
    oled.setCursor(0,40);
    oled.print("PROGRESS:");
    oled.print(percent);
    oled.println("%");

    oled.display();
}

// 距離維持ゲーム画面表示
void showDistanceGame(int targetDistance, int currentDistance){
    oled.clearDisplay();

    // テキスト設定
    oled.setTextColor(WHITE);
    oled.setTextSize(TEXTSIZE_SMALL);

    // 目標距離
    oled.setCursor(0, 0);
    oled.print("TARGET");
    oled.setCursor(0, 12);
    oled.print(targetDistance);
    oled.print(" cm");

    // 現在の距離
    oled.setCursor(0, 36);
    oled.print("CURRENT");
    oled.setCursor(0, 48);
    oled.print(currentDistance);
    oled.print(" cm");

    oled.display();
}

