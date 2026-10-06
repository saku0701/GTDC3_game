#include <Arduino.h>
#include "MiniGameManager.h"

// ミニゲーム抽選
MiniGameType selectMiniGame(void){
  return (MiniGameType)random(0, 5);
  // return (MiniGameType)2;
}