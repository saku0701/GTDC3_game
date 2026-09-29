#include <Arduino.h>
#include "MiniGame.h"

static MiniGameType currentGame;
static Difficulty currentDifficulty;

static bool successFlag;
static bool failedFlag;

static unsigned long startTime;

static bool timeUpFlag;

static unsigned long timeLimit;

// 長押し対策
static bool prevLeftPressed = false;
static bool prevCenterPressed = false;
static bool prevRightPressed = false;

// PUSHゲームのボタン抽選用
static int targetButton;

// PUSH＿INODERゲームのボタン順抽選
static int currentIndex;
static int targetCount;
static int targetOrder[4];

// JOYSTICK_DOWNゲーム用
static int targetAngle;
static int angleTolerance;

// ゲーム指示
static char instruction[32];

// 難易度ごとの制限時間
#define EASY_TIME_LIMIT (10 * 1000)
#define NORMAL_TIME_LIMIT (7 * 1000)
#define HARD_TIME_LIMIT (3 * 1000)


enum ButtonType{
    BUTTON_LEFT,
    BUTTON_CENTER,
    BUTTON_RIGHT
};

// 角度取得関数
float getJoystickAngle(){
    int x = analogRead(A0);
    int y = analogRead(A1);

    float dx = (float)x - 512.0;
    float dy = (float)y - 512.0;

    float angle =
        atan2(dy, dx) * 180.0 / PI;

    if(angle < 0)
    {
        angle += 360.0;
    }

    return angle;
}

// 角度判定関数
bool isAngleMatch(float actual, float target, float tolerance){
    float diff =
        fabs(actual - target);

    if(diff > 180){
        diff = 360 - diff;
    }

    return (diff <= tolerance);
}

// PUSHゲーム
void createPushInstruction(){
    // ボタン抽選
    targetButton = random(0, 3);
    // Serial.println(instruction);

    switch(currentDifficulty){
        case EASY:

            if(targetButton == BUTTON_LEFT){
                // strcpy(instruction, "ひだり");
                strcpy(instruction, "LEFT");
            } 

            if(targetButton == BUTTON_CENTER){
                // strcpy(instruction, "まんなか");
                strcpy(instruction, "CENTER");
            }

            if(targetButton == BUTTON_RIGHT){
                // strcpy(instruction, "みぎ");
                strcpy(instruction, "RIGHT");
            }

            break;

        case NORMAL:

            if(targetButton == BUTTON_LEFT){
                // strcpy(instruction, "みどり");
                strcpy(instruction, "GREEN");
            }
                
            if(targetButton == BUTTON_CENTER){
                // strcpy(instruction, "きいろ");
                strcpy(instruction, "YELLOW");
            }

            if(targetButton == BUTTON_RIGHT){
                // strcpy(instruction, "あか");
                strcpy(instruction, "RED");
            }
                
            break;

        case HARD:

            if(targetButton == BUTTON_LEFT){
                // strcpy(instruction, "きいろのひだり");
                strcpy(instruction, "LEFT  of YELLOW");
            }
                
            if(targetButton == BUTTON_CENTER){
                // strcpy(instruction, "みどりのみぎ");
                strcpy(instruction, "RIGHT of GREEN");
            }

            if(targetButton == BUTTON_RIGHT){
                // strcpy(instruction, "きいろのみぎ");
                strcpy(instruction, "RIGHT of YELLOW");
            }
                
            break;
    }
    Serial.print("Instruction=");
    Serial.println(instruction);
}

// PUSH＿INODERゲーム
void createPushInorderInstruction(){
    currentIndex = 0;

    switch(currentDifficulty){
        case EASY:

            targetCount = 2;
            break;

        case NORMAL:

            targetCount = 3;
            break;

        case HARD:

            targetCount = 4;
            break;
    }

    instruction[0] = '\0';

    for(int i = 0; i < targetCount; i++){
        targetOrder[i] = random(0, 3);

        switch(targetOrder[i]){
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
}

// JOYSTICK_DOWNゲーム
void createJoystickDownInstruction(){
    targetAngle = random(0, 36) * 10;

    switch(currentDifficulty){
        case EASY:
            angleTolerance = 45;
            break;

        case NORMAL:
            angleTolerance = 25;
            break;

        case HARD:
            angleTolerance = 10;
            break;
    }

    sprintf(instruction, "%03d DEG", targetAngle);
}

// ミニゲーム実行
void startMiniGame(MiniGameType gameType, Difficulty difficulty){
    currentGame = gameType;
    currentDifficulty = difficulty;

    // ゲームクリア判定フラグ
    successFlag = false;
    failedFlag = false;
    timeUpFlag = false;

    // ゲーム開始時間取得
    startTime = millis();

    switch(currentDifficulty){
        case EASY:
            timeLimit = EASY_TIME_LIMIT;
            break;

        case NORMAL:
            timeLimit = NORMAL_TIME_LIMIT;
            break;

        case HARD:
            timeLimit = HARD_TIME_LIMIT;
            break;
    }


    switch(currentGame){
        case GAME_PUSH:
            createPushInstruction();
            break;

        case GAME_PUSH_INORDER:
            createPushInorderInstruction();
            break;

        case GAME_JOYSTICK_DOWN:
            createJoystickDownInstruction();
            break;
    }
}


// 入力状態
void updateMiniGame(){
    /* 制限時間監視 */
    if(millis() - startTime >= timeLimit){
        timeUpFlag = true;
        return;
    }

    // 入力状態取得
    bool leftPressed   = digitalRead(2) == LOW;
    bool centerPressed = digitalRead(3) == LOW;
    bool rightPressed  = digitalRead(4) == LOW;
    float currentAngle = getJoystickAngle();

    switch(currentGame){
        case GAME_PUSH://ボタン押しゲーム
            if(leftPressed){
                Serial.println("L PUSH");
                if(targetButton == BUTTON_LEFT)
                    successFlag = true;
                else
                    failedFlag = true;
            }

            if(centerPressed){
                Serial.println("C PUSH");
                if(targetButton == BUTTON_CENTER)
                    successFlag = true;
                else
                    failedFlag = true;
            }

            if(rightPressed){
                Serial.println("R PUSH");
                if(targetButton == BUTTON_RIGHT)
                    successFlag = true;
                else
                    failedFlag = true;
            }
            break;

        case GAME_PUSH_INORDER:{//ボタン順番押しゲーム
            int pressedButton = -1;

            // 押された瞬間のみ検出
            if(leftPressed && !prevLeftPressed){
                pressedButton = BUTTON_LEFT;
            }

            if(centerPressed && !prevCenterPressed){
                pressedButton = BUTTON_CENTER;
            }

            if(rightPressed && !prevRightPressed){
                pressedButton = BUTTON_RIGHT;
            }

            prevLeftPressed = leftPressed;
            prevCenterPressed = centerPressed;
            prevRightPressed = rightPressed;

            if(pressedButton == -1){
                return;
            }

            Serial.print("Input=");
            Serial.println(pressedButton);

            if(pressedButton == targetOrder[currentIndex]){
                currentIndex++;

                Serial.println("Correct");

                if(currentIndex >= targetCount){
                    successFlag = true;
                }
            }
            else{
                Serial.println("Wrong");
                failedFlag = true;
            }
            break;
        }

        case GAME_JOYSTICK_DOWN://ジョイスティック倒しゲーム
            // これはデバッグ用表示
            Serial.print("Target=");
            Serial.print(targetAngle);
            Serial.print(" Current=");
            Serial.println(currentAngle);

            if(isAngleMatch(currentAngle, targetAngle, angleTolerance)){
                successFlag = true;
            }
    }
}

bool isMiniGameSuccess(){
    return successFlag;
}

bool isMiniGameFailed(){
    return failedFlag;
}

const char* getInstruction(){
    return instruction;
}

bool isTimeUp(){
    return timeUpFlag;
}