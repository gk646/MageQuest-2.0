#ifndef MAGE_QUEST_SRC_UI_PLAYER_PLAYERUI_H_
#define MAGE_QUEST_SRC_UI_PLAYER_PLAYERUI_H_

#include "game/MiniMap.h"
#include "game/RegionMap.h"
#include "player/CharacterPanel.h"
#include "player/CharacterBag.h"
#include "player/StatusBar.h"
#include "player/QuestPanel.h"

struct PlayerUI {
  RegionMap region_map{};
  MiniMap mini_map{&region_map.isWindowOpen};
  CharacterPanel char_panel{};
  CharacterBag charBag{};
  StatusBar status_bar{};
  QuestPanel questPanel{};
  HotBar playerHotbar{};

  void Draw() noexcept {
    DRAW_NPC_DIALOGUE()
    PLAYER_EFFECTS.Draw();
    playerHotbar.Draw();
    questPanel.Draw();
    status_bar.Draw();
    mini_map.Draw();
    char_panel.Draw();
    charBag.Draw();
    region_map.Draw();
    DrawDraggedAndToolTip();
  }

  void Update() noexcept {
    WINDOW_FOCUSED = false;
    questPanel.Update();
    mini_map.Update();
    status_bar.Update();
    char_panel.update();
    charBag.Update();
    region_map.Update();
    playerHotbar.Update();
    Util::update();
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
    if (char_panel.isWindowOpen || charBag.isWindowOpen || region_map.isWindowOpen) {
      char_panel.isWindowOpen = false;
      charBag.isWindowOpen = false;
      region_map.isWindowOpen = false;
      return true;
    } else {
      return false;
    }
  }
  inline void ResetPosition() noexcept {
    char_panel.ResetPosition();
    charBag.ResetPosition();
    questPanel.ResetPosition();
    region_map.ResetPosition();
  }
};

#endif  //MAGE_QUEST_SRC_UI_PLAYER_PLAYERUI_H_
