#include <Arduino.h>
#include "MiniGame.h"

static MiniGameType currentGame;
static Difficulty currentDifficulty;

static bool successFlag;
static bool failedFlag;

static unsigned long startTime;

static bool timeUpFlag;

static unsigned long timeLimit;

// ジョイスティック不感帯 
#define JOYSTICK_DEADZONE 510

// 測距センサ
#define TRIG_PIN 9
#define ECHO_PIN 8
#define DISTANCE_TOLERANCE 3
#define EASY_HOLD_TIME    1000
#define NORMAL_HOLD_TIME  2000
#define HARD_HOLD_TIME    3000

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
static bool joystickJudged;

// DISTANCEゲーム用
static int targetDistance;
static int distanceTolerance;
static unsigned long requiredHoldTime;
static unsigned long accumulatedHoldTime;
static unsigned long lastUpdateTime;
static float currentDistanceCm;

// JOYSTICK_ROTATEゲーム用
static int targetRotateCount;
static int completedRotateCount;
static float currentRotateProgress;
static bool rotateInit;
static float lastRotateAngle;
static float accumulatedRotateAngle;


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
    float dy = 512.0 - (float)y;

    float angle = atan2(dy, dx) * 180.0 / PI;

    if(angle < 0){
        angle += 360.0;
    }

    return angle;
}

// 傾き量判定関数
bool isJoystickTilted(){
    int x = analogRead(A0);
    int y = analogRead(A1);

    float dx = (float)x - 512.0;
    float dy = (float)y - 512.0;

    float distance =
        sqrt(dx * dx + dy * dy);

    return distance > JOYSTICK_DEADZONE;
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

// 距離取得関数
float getDistanceCm(){
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);

    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH, 30000);

    float distance = duration / 58.00;  // Formula: (340m/s * 1us) / 2

    return distance;
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

// Distanceゲーム
void createDistanceInstruction(){
    targetDistance = random(10, 40);

    distanceTolerance = DISTANCE_TOLERANCE;

    accumulatedHoldTime = 0;

    lastUpdateTime = millis();

    switch(currentDifficulty){
        case EASY:
            requiredHoldTime = EASY_HOLD_TIME;
            break;

        case NORMAL:
            requiredHoldTime = NORMAL_HOLD_TIME;
            break;

        case HARD:
            requiredHoldTime = HARD_HOLD_TIME;
            break;
    }

    sprintf(instruction, "%d cm", targetDistance);
}

// JOYSTICK＿ROTATEゲーム
void createJoystickRotateInstruction()
{
    rotateInit = false;

    completedRotateCount = 0;

    accumulatedRotateAngle = 0.0;

    currentRotateProgress = 0.0;

    switch(currentDifficulty){
        case EASY:
            targetRotateCount = 3;
            break;

        case NORMAL:
            targetRotateCount = 5;
            break;

        case HARD:
            targetRotateCount = 10;
            break;
    }

    sprintf(instruction, "ROTATE %d", targetRotateCount);
}

// ミニゲーム実行
void startMiniGame(MiniGameType gameType, Difficulty difficulty){
    currentGame = gameType;
    currentDifficulty = difficulty;

    // ゲームクリア判定フラグ
    successFlag = false;
    failedFlag = false;
    timeUpFlag = false;
    joystickJudged = false;

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

        case GAME_DISTANCE_KEEP:
            createDistanceInstruction();
            break;

        case GAME_JOYSTICK_ROTATE:
            createJoystickRotateInstruction();
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
            if(joystickJudged){
                break;
            }

            // 不感帯内は無視
            if(!isJoystickTilted()){
                break;
            }

            // デバッグ用表示
            Serial.print("Target=");
            Serial.print(targetAngle);
            Serial.print(" Current=");
            Serial.println(currentAngle);

            if(isAngleMatch(currentAngle, targetAngle, angleTolerance)){
                successFlag = true;

                Serial.println("Correct");
            }
            else{
                failedFlag = true;

                Serial.println("Wrong");
            }

            joystickJudged = true;

            break;

        case GAME_DISTANCE_KEEP:{//距離保持ゲーム
                currentDistanceCm = getDistanceCm();

                float currentDistance = currentDistanceCm;

                unsigned long now = millis();

                if(
                    currentDistance >= targetDistance - distanceTolerance
                    &&
                    currentDistance <= targetDistance + distanceTolerance
                ){
                    accumulatedHoldTime += (now - lastUpdateTime);
                }

                lastUpdateTime = now;

                if(accumulatedHoldTime >= requiredHoldTime){
                    successFlag = true;
                }

                break;
            }

            case GAME_JOYSTICK_ROTATE:{//ジョイスティック回転ゲーム
                if(!isJoystickTilted()){
                    break;
                }

                float currentAngle = getJoystickAngle();

                if(!rotateInit){
                    lastRotateAngle = currentAngle;

                    rotateInit = true;

                    break;
                }

                float delta = currentAngle - lastRotateAngle;

                if(delta > 180){
                    delta -= 360;
                }

                if(delta < -180){
                    delta += 360;
                }

                lastRotateAngle = currentAngle;

                // 時計回りのみ加算
                if(delta < 0){
                    accumulatedRotateAngle += -delta;

                    currentRotateProgress = accumulatedRotateAngle;
                }

                if(accumulatedRotateAngle >= 360.0){
                    accumulatedRotateAngle -= 360.0;

                    completedRotateCount++;

                    Serial.print("Rotate=");
                    Serial.println(completedRotateCount);
                }

                if(completedRotateCount >= targetRotateCount){
                    successFlag = true;
                }

                break;
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

// 角度
int getTargetAngle(){
    return targetAngle;
}

int getCurrentAngle(){
    return (int)getJoystickAngle();
}

// 測距
int getTargetDistance(){
    return targetDistance;
}

int getCurrentDistance(){

    Serial.print("Distance=");
    Serial.println(currentDistanceCm);

    return (int)currentDistanceCm;
}

// ジョイスティック回転
int getTargetRotateCount(){
    return targetRotateCount;
}

int getCompletedRotateCount(){
    return completedRotateCount;
}

int getCurrentRotatePercent(){
    return (int)(accumulatedRotateAngle * 100.0 / 360.0);
}



MiniGameType getCurrentGame(){
return currentGame;
}

bool isTimeUp(){
    return timeUpFlag;
}