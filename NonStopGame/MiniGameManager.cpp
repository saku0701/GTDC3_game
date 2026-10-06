#include <Arduino.h>
#include "MiniGameManager.h"

// ミニゲーム抽選
MiniGameType selectMiniGame(void){
  return (MiniGameType)random(0, GAME_TYPE_COUNT);
  // return (MiniGameType)2;
}