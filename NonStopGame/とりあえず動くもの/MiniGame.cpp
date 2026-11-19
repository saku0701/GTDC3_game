#include <Arduino.h>
#include "MiniGame.h"

static MiniGameType currentGame;
static Difficulty currentDifficulty;

// 現在のゲームスピード
static GameSpeed currentSpeed;

static bool successFlag;
static bool failedFlag;

static unsigned long startTime;

static bool timeUpFlag;

static unsigned long timeLimit;

// ボタン入力ピン
#define BUTTON_LEFT_PIN       2
#define BUTTON_CENTER_PIN     3
#define BUTTON_RIGHT_PIN      4
#define PUSH_ORDER_MAX_COUNT 4

// ボタン順押し
#define PUSH_ORDER_EASY_COUNT 2
#define PUSH_ORDER_NORMAL_COUNT 3
#define PUSH_ORDER_HARD_COUNT 4

// ジョイスティック
#define JOYSTICK_X_PIN A0
#define JOYSTICK_Y_PIN A1
#define JOYSTICK_CENTER_X 512.0f
#define JOYSTICK_CENTER_Y 512.0f

// ジョイスティック不感帯 
#define JOYSTICK_DEADZONE 510

// ジョイスティック回転角
#define FULL_CIRCLE_DEGREES 360.0f
#define HALF_CIRCLE_DEGREES 180.0f

// ジョイスティック角度範囲指定
#define TARGET_ANGLE_STEP 10
#define TARGET_ANGLE_COUNT 36

// ジョイスティック難易度ごとの角度許容範囲
#define EASY_ANGLE_TOLERANCE 45
#define NORMAL_ANGLE_TOLERANCE 25
#define HARD_ANGLE_TOLERANCE 10

// ジョイスティック難易度ごとの回転数
#define EASY_ROTATE_COUNT 3
#define NORMAL_ROTATE_COUNT 5
#define HARD_ROTATE_COUNT 7

// 回転最大割合(100.0%)
#define ROTATE_PERCENT_MAX 100.0f

// 測距センサ
#define TRIG_PIN 9
#define ECHO_PIN 8

// 測距パラメータ
#define DISTANCE_TOLERANCE_CM 3
#define DISTANCE_TARGET_MIN_CM 10
#define DISTANCE_TARGET_MAX_CM 40

// 測距ゲーム保持時間
#define EASY_HOLD_TIME    1000
#define NORMAL_HOLD_TIME  2000
#define HARD_HOLD_TIME    2500

// 測距計測パラメータ
#define DISTANCE_ECHO_TIMEOUT_US 30000UL
#define DISTANCE_CONVERSION_VALUE 58.0f

// 長押し対策
static bool prevLeftPressed = false;
static bool prevCenterPressed = false;
static bool prevRightPressed = false;

// PUSHゲームのボタン抽選用
static int targetButton;

// PUSH＿INODERゲームのボタン順抽選
static int currentIndex;
static int targetCount;
static int targetOrder[PUSH_ORDER_MAX_COUNT];

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

// スピードごとの制限時間
#define SLOW_TIME_LIMIT    (10UL * 1000UL)
#define NORMAL_TIME_LIMIT   (7UL * 1000UL)
#define FAST_TIME_LIMIT     (3UL * 1000UL)

// 残り時間表示
unsigned long getElapsedTime(){
    return millis() - startTime;
}
unsigned long getTimeLimit(){
    return timeLimit;
}

// ボタン
enum ButtonType{
    BUTTON_LEFT,
    BUTTON_CENTER,
    BUTTON_RIGHT
};

// 角度取得関数
float getJoystickAngle(){
    int x = analogRead(JOYSTICK_X_PIN);
    int y = analogRead(JOYSTICK_Y_PIN);

    float dx = (float)x - JOYSTICK_CENTER_X;
    float dy = JOYSTICK_CENTER_Y - (float)y;

    float angle = atan2(dy, dx) * HALF_CIRCLE_DEGREES / PI;

    if(angle < 0){
        angle += FULL_CIRCLE_DEGREES;
    }

    return angle;
}

// 傾き量判定関数
bool isJoystickTilted(){
    int x = analogRead(JOYSTICK_X_PIN);
    int y = analogRead(JOYSTICK_Y_PIN);

    float dx = (float)x - JOYSTICK_CENTER_X;
    float dy = (float)y - JOYSTICK_CENTER_Y;

    float distance = sqrt(dx * dx + dy * dy);

    return distance > JOYSTICK_DEADZONE;
}

// 角度判定関数
bool isAngleMatch(float actual, float target, float tolerance){
    float diff =
        fabs(actual - target);

    if(diff > HALF_CIRCLE_DEGREES){
        diff = FULL_CIRCLE_DEGREES - diff;
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

    long duration = pulseIn(ECHO_PIN, HIGH, DISTANCE_ECHO_TIMEOUT_US);

    float distance = duration / DISTANCE_CONVERSION_VALUE;  // Formula: (340m/s * 1us) / 2

    return distance;
}

// PUSHゲーム
void createPushInstruction(){
    // ボタン抽選
    targetButton = random(0, 3);

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
            targetCount = PUSH_ORDER_EASY_COUNT;
            break;

        case NORMAL:
            targetCount = PUSH_ORDER_NORMAL_COUNT;
            break;

        case HARD:
            targetCount = PUSH_ORDER_HARD_COUNT;
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
    targetAngle = random(0, TARGET_ANGLE_COUNT) * TARGET_ANGLE_STEP;

    switch(currentDifficulty){
        case EASY:
            angleTolerance = EASY_ANGLE_TOLERANCE;
            break;

        case NORMAL:
            angleTolerance = NORMAL_ANGLE_TOLERANCE;
            break;

        case HARD:
            angleTolerance = HARD_ANGLE_TOLERANCE;
            break;
    }

    sprintf(instruction, "%03d DEG", targetAngle);
}

// Distanceゲーム
void createDistanceInstruction(){
    targetDistance = random(DISTANCE_TARGET_MIN_CM, DISTANCE_TARGET_MAX_CM);

    distanceTolerance = DISTANCE_TOLERANCE_CM;

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
void createJoystickRotateInstruction(){
    rotateInit = false;

    completedRotateCount = 0;

    accumulatedRotateAngle = 0.0;

    currentRotateProgress = 0.0;

    switch(currentDifficulty){
        case EASY:
            targetRotateCount = EASY_ROTATE_COUNT;
            break;

        case NORMAL:
            targetRotateCount = NORMAL_ROTATE_COUNT;
            break;

        case HARD:
            targetRotateCount = HARD_ROTATE_COUNT;
            break;
    }

    sprintf(instruction, "ROTATE %d", targetRotateCount);
}

// ミニゲーム実行
void startMiniGame(MiniGameType gameType, Difficulty difficulty, GameSpeed speed){
    // 実行するミニゲームを保存する
    currentGame = gameType;

    // ゲーム内容の難易度を保存する
    currentDifficulty = difficulty;

    // ゲームの制限時間設定を保存する
    currentSpeed = speed;

    // ゲームクリア判定フラグ
    successFlag = false;
    failedFlag = false;
    timeUpFlag = false;
    joystickJudged = false;

    // ゲーム開始時間取得
    startTime = millis();

    // スピード設定に応じて制限時間を決定する
    switch(currentSpeed){
        case SPEED_SLOW:
            // SLOWは制限時間10秒
            timeLimit = SLOW_TIME_LIMIT;
            break;

        case SPEED_NORMAL:
            // NORMALは制限時間7秒
            timeLimit = NORMAL_TIME_LIMIT;
            break;

        case SPEED_FAST:
            // FASTは制限時間3秒
            timeLimit = FAST_TIME_LIMIT;
            break;

        default:
            // 不正な設定値の場合はNORMALを代替値とする
            currentSpeed = SPEED_NORMAL;
            timeLimit = NORMAL_TIME_LIMIT;
            break;
    }

    // デバッグ用：開始時の設定値をシリアルモニタへ表示する
    Serial.print("Difficulty=");
    Serial.print((int)currentDifficulty);

    Serial.print(" Speed=");
    Serial.print((int)currentSpeed);

    Serial.print(" TimeLimit=");
    Serial.println((int)timeLimit);


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
    bool leftPressed   = digitalRead(BUTTON_LEFT_PIN) == LOW;
    bool centerPressed = digitalRead(BUTTON_CENTER_PIN) == LOW;
    bool rightPressed  = digitalRead(BUTTON_RIGHT_PIN) == LOW;
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

                if(delta > HALF_CIRCLE_DEGREES){
                    delta -= FULL_CIRCLE_DEGREES;
                }

                if(delta < -HALF_CIRCLE_DEGREES){
                    delta += FULL_CIRCLE_DEGREES;
                }

                lastRotateAngle = currentAngle;

                // 時計回りのみ加算
                if(delta < 0){
                    accumulatedRotateAngle += -delta;

                    currentRotateProgress = accumulatedRotateAngle;
                }

                if(accumulatedRotateAngle >= FULL_CIRCLE_DEGREES){
                    accumulatedRotateAngle -= FULL_CIRCLE_DEGREES;

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
    return (int)(accumulatedRotateAngle * ROTATE_PERCENT_MAX / FULL_CIRCLE_DEGREES);
}



MiniGameType getCurrentGame(){
return currentGame;
}

bool isTimeUp(){
    return timeUpFlag;
}