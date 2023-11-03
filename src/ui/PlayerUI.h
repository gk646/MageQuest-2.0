#ifndef MAGE_QUEST_SRC_UI_PLAYER_PLAYERUI_H_
#define MAGE_QUEST_SRC_UI_PLAYER_PLAYERUI_H_

#include "game/MiniMap.h"
#include "game/RegionMap.h"
#include "player/CharacterPanel.h"
#include "player/CharacterBag.h"
#include "player/StatusBar.h"
#include "player/QuestPanel.h"
#include "player/TalentPanel.h"

struct PlayerUI {
  TalentPanel talentPanel;
  RegionMap regionMap{};
  MiniMap miniMap{&regionMap.isWindowOpen};
  CharacterPanel charPanel{};
  CharacterBag charBag{};
  StatusBar statusBar{};
  QuestPanel questPanel{};
  HotBar playerHotbar{};

  void Draw() noexcept {
    DRAW_NPC_DIALOGUE()
    PLAYER_EFFECTS.Draw();
    playerHotbar.Draw();
    miniMap.Draw();
    statusBar.Draw();
    questPanel.Draw();
    charBag.Draw();
    charPanel.Draw();
    regionMap.Draw();
    talentPanel.Draw();
    DrawDraggedAndToolTip();
  }
  void Update() noexcept {
    WINDOW_FOCUSED = false;
    questPanel.Update();
    miniMap.Update();
    statusBar.Update();
    charPanel.update();
    charBag.Update();
    regionMap.Update();
    talentPanel.Update();
    playerHotbar.Update();
    Util::Update();
    update_special_items();
  }
  static inline void DrawDraggedAndToolTip() noexcept {
    if (DRAGGED_ITEM) {
      DRAGGED_ITEM->Draw({MOUSE_POS.x - 22, MOUSE_POS.y - 22, 45, 45});
    }
    if (TOOL_TIP_ITEM) {
      TOOL_TIP_ITEM->DrawToolTip();
      TOOL_TIP_ITEM = nullptr;
    }
  }
  static inline void update_special_items() noexcept {
    if (DRAGGED_ITEM && !IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      if (WINDOW_FOCUSED) {
        InventorySlot::RecoverDraggedItem();
      } else {
        WORLD_OBJECTS.push_back(new DroppedItem(
            {PLAYER_X + PLAYER.size.x / 2 + 50, PLAYER_Y + PLAYER.size.y / 2},
            DRAGGED_ITEM));
        DRAGGED_ITEM = nullptr;
        DRAGGED_SLOT = nullptr;
      }
    }
  }
  inline bool window_closeable() noexcept {
    if (charPanel.isWindowOpen || charBag.isWindowOpen || regionMap.isWindowOpen ||
        questPanel.isWindowOpen) {
      charPanel.isWindowOpen = false;
      charBag.isWindowOpen = false;
      regionMap.isWindowOpen = false;
      questPanel.isWindowOpen = false;
      return true;
    } else {
      return false;
    }
  }
  inline void ResetPosition() noexcept {
    charPanel.ResetPosition();
    charBag.ResetPosition();
    questPanel.ResetPosition();
    regionMap.ResetPosition();
  }
};

#endif  //MAGE_QUEST_SRC_UI_PLAYER_PLAYERUI_H_
