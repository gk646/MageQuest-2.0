#ifndef MAGEQUEST_SRC_GAMEOBJECTS_NPCS_TRADER_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_NPCS_TRADER_H_

#include "../../ui/elements/TradeWIndow.h"

struct Trader : public NPC {
  std::vector<ItemSlot> inventory;
  int coins;
  Window tradeWindow;
  Trader(const Point& pos, MonsterResource* res, Zone zone, NPC_ID id, float speed)
      : NPC(pos, res, zone, id, speed),
        coins(PLAYER_STATS.level * 60),
        tradeWindow(0, 0, 450, 450, 15, name.c_str(), 0, sound::EMPTY_SOUND,
                    sound::EMPTY_SOUND) {}


  void OpenTradeWindow() { tradeWindow.OpenWindow(); }

  inline void InteractWithTrader() {
    if (Util::EPressed() && currentZone == CURRENT_ZONE && this->Intersects(PLAYER)) {
      if (!dialogue) {
        if (Quests::TRADING->objectives[0]->Progress(this)) {
          tradeWindow.OpenWindow();
        }
        dialogueProgressCount = 0;
        dialogueShowDelayTicks = 300;
      } else if (dialogueShowDelayTicks < 0) {
        dialogueProgressCount = 0;
        dialogueShowDelayTicks = 300;
      } else if (dialogueProgressCount < 1000) {
        dialogueProgressCount = 1000;
      } else {
        if (Quests::TRADING->objectives[0]->Progress(this)) {
          tradeWindow.OpenWindow();
        }
      }
    }
  }
};

#endif  //MAGEQUEST_SRC_GAMEOBJECTS_NPCS_TRADER_H_
