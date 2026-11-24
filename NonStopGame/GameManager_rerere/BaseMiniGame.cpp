/*
ベースミニゲーム
ミニゲームの抽選、値計算、初期化、状態更新
*/

#include "BaseMiniGame.h"

extern bool previousLeftPressed;
extern bool previousCenterPressed;
extern bool previousRightPressed;

// ミニゲーム抽選
MiniGameType selectMiniGame(void){
  return (MiniGameType)random(0, GAME_TYPE_COUNT);
 }

// ジョイスティック角度取得
float getJoystickAngle(){
    float angle = atan2(dy, dx) * HALF_CIRCLE_DEGREES / PI;

    if(angle < 0){
        angle += FULL_CIRCLE_DEGREES;
    }

    return angle;
}

// ジョイスティック傾き量判定
bool isJoystickTilted(){
    float dx = (float)x - JOYSTICK_CENTER_X;
    float dy = (float)y - JOYSTICK_CENTER_Y;

    float distance = sqrt(dx * dx + dy * dy);

    return distance > JOYSTICK_DEADZONE;
}

// ジョイスティック回転割合取得
int getRotatePercent(){
    float diff = fabs(actual - target);

    if(diff > HALF_CIRCLE_DEGREES){
        diff = FULL_CIRCLE_DEGREES - diff;
    }

    return (diff <= tolerance);
}

// 指定ボタン押下ゲーム
// 初期化
void initPushGame(){
    // ボタン抽選
    targetButton = random(0, 3);

    switch (targetButton);
    {
        case BUTTON_LEFT:
            targetButton = "L";
            break;
        case BUTTON_CENTER:
            targetButton = "C";
            break;
        case BUTTON_RIGHT:
            targetButton = "R";
            break;
        
        // default:
        //     targetButton = "C";
        //     break;
    }
}
// 状態更新
void updatePushGame(){
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
}

// ボタン指定順押下ゲーム
// 初期化
void initPushInOrderGame(){
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
// 状態更新
void updatePushInOrderGame(){
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
}

// ジョイスティック傾倒ゲーム
// 初期化
void initJoystickDownGame(){
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
// 状態更新
void updateJoystickDownGame(){
    if(joystickJudged){
        return;
    }

    // 不感帯内は無視
    if(!isJoystickTilted()){
        return;
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
}

// ジョイスティック回転ゲーム
// 初期化
void initJoystickRotateGame(){
    rotateInit;
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
// 状態更新
void updateJoystickRotateGame();

// 距離維持ゲーム
// 初期化
void initDistanceGame(){
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
// 状態更新
void updateDistanceGame(){
    if(!isJoystickTilted()){
       return;
    }

    // 角度を取得
    float currentAngle = getJoystickAngle();

    // 回転開始判定
    if(!rotateInit){
        lastRotateAngle = currentAngle;
        rotateInit = true;
        return;
    }

    // 角度変位
    float delta = currentAngle - lastRotateAngle;

    // 角度を-180~180に収める
    if(delta > HALF_CIRCLE_DEGREES){
        delta -= FULL_CIRCLE_DEGREES;
    }
    if(delta < -HALF_CIRCLE_DEGREES){
        delta += FULL_CIRCLE_DEGREES;
    }

    // 角度の保存
    lastRotateAngle = currentAngle;

    // 時計回りのみ加算
    if(delta < 0){
        accumulatedRotateAngle += -delta;
        currentRotateProgress = accumulatedRotateAngle;
    }

        // 回転完了数を加算
        if(accumulatedRotateAngle >= FULL_CIRCLE_DEGREES){
        accumulatedRotateAngle -= FULL_CIRCLE_DEGREES;

        completedRotateCount++;

        Serial.print("Rotate=");
        Serial.println(completedRotateCount);
    }

    if(completedRotateCount >= targetRotateCount){
        successFlag = true;
    }
}