#include "MiniGameManager.h"
#include "MenuManager.h"
#include "DisplayManager.h"
#include "InputManager.h"
#include "BaseMiniGame.h"
#include "GameManager.h"


/*グローバル変数*/
// 全体状態管理
GameState currentState = MENU_INIT;

// GameManagerへ通知する開始要求
bool startRequested = false;

// ゲーム設定
MiniGameType currentGame;
Difficulty currentDifficulty;
GameSpeed currentSpeed;

// ミニゲーム成功失敗フラグ
bool successFlag;
bool failedFlag;
unsigned long startTime;
bool timeUpFlag;

// 時間制限
unsigned long timeLimit;

// ========================================
// ボタン状態
// ========================================
// 現在の入力状態
bool leftPressed = false;
bool centerPressed = false;
bool rightPressed = false;

// 前回の入力状態
bool previousLeftPressed = false;
bool previousCenterPressed = false;
bool previousRightPressed = false;

// 押下開始状態
bool leftTriggered = false;
bool centerTriggered = false;
bool rightTriggered = false;

// ========================================
// ジョイスティック状態
// ========================================
int joystickX = 0;
int joystickY = 0;

// ========================================
// 測距センサ状態
// ========================================
float currentDistanceCm = DISTANCE_ERROR_VALUE;
unsigned long lastDistanceUpdateTime = 0;

// 長押し対策
bool prevLeftPressed = false;
bool prevCenterPressed = false;
bool prevRightPressed = false;

// PUSHゲームのボタン抽選用
int targetButton;

// PUSH＿INODERゲームのボタン順抽選
int currentIndex;
int targetCount;
int targetOrder[PUSH_ORDER_MAX_COUNT];

// JOYSTICK_DOWNゲーム用
int targetAngle;
int angleTolerance;
bool joystickJudged;

// DISTANCEゲーム用
int targetDistance;
int distanceTolerance;
unsigned long requiredHoldTime;
unsigned long accumulatedHoldTime;
unsigned long lastUpdateTime;
float currentDistanceCm;

// JOYSTICK_ROTATEゲーム用
int targetRotateCount;
int completedRotateCount;
float currentRotateProgress;
bool rotateInit;
float lastRotateAngle;
float accumulatedRotateAngle;


// ゲーム指示
char instruction[32];


bool startRequested = false;
State currentState = MENU_INIT;

MiniGameType currentGame;


void setup(){
    Serial.begin(115200);
    randomSeed(micros());
    initInputManager();
    initDisplay();
    initMemu();
}

void loop(){
    switch (currentState)
    {
        case MENU_INIT: // メニュー初期化
            initMenu();
            currentState = MENU;
            break;
            
        case MENU: //メニュー
            updateMenu();
            if (startRequested == true)
            {
                currentState = GAME_INIT;
            }            
            break;

        case GAME_INIT: // ゲーム開始時の初期化
            startMiniGame(MiniGameType gameType, Difficulty dificulty, GameSpeed speed);
            currentState = STATE_Instruction;
            break;

        case STATE_Instruction: // ゲーム指示表示
            currentState = GAME_MINIGAME;
            break;

        case GAME_MINIGAME:
            updateMiniGame();
            if (currentGameCount > targetGameCount)
            {
                currentState = STATE_GAMECLEAR;
            }
            else if(successFlag == true)
            {
                currentState = STATE_SUCCESS;
            }
            else if (failedFlag == true || timeUpFlag == true)
            {
                currentState = STATE_GAMEOVER;
            }
            else
            {
                
            }
            break;

        case STATE_SUCCESS:
            currentState = GAME_INIT;
            break;

        case STATE_GAMECLEAR:
            currentState = GAME_INIT;
            break;
            
        case STATE_GAMEOVER:
            currentState = GAME_INIT;
            break;

        default:
            break;
    }
}