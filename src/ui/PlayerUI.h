#ifndef MAGE_QUEST_SRC_UI_PLAYER_PLAYERUI_H_
#define MAGE_QUEST_SRC_UI_PLAYER_PLAYERUI_H_

#include "player/elements/ToolTip.h"

#include "game/MiniMap.h"
#include "player/CharacterPanel.h"
#include "player/StatusBar.h"

struct PlayerUI {
  MiniMap mini_map{};
  CharacterPanel char_panel{};
  CharacterBag char_bag{};
  StatusBar status_bar{};

  void draw() noexcept {
    PLAYER_EFFECTS.draw();
    PLAYER_HOTBAR.draw();
    status_bar.draw();
    mini_map.draw();
    char_panel.draw();
    char_bag.draw();
    draw_special_items();
  }
  void update() noexcept {
    WINDOW_FOCUSED = false;
    mini_map.update();
    status_bar.update();
    char_panel.update();
    char_bag.update();
    update_special_items();
  }
  static inline void draw_special_items() noexcept {
    if (DRAGGED_ITEM) {
      DRAGGED_ITEM->draw({GetMousePosition().x - 22, GetMousePosition().y - 22, 45, 45});
    }
    if (TOOL_TIP_ITEM) {
      TOOL_TIP_ITEM->draw_tooltip();
      TOOL_TIP_ITEM = nullptr;
    }
  }
  static inline void update_special_items() noexcept {
    if (DRAGGED_ITEM && !IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      if (WINDOW_FOCUSED) {
        InventorySlot::place_item_back();
      } else {
        WORLD_OBJECTS.push_back(new DroppedItem(PLAYER.pos, DRAGGED_ITEM));
        DRAGGED_ITEM = nullptr;
        DRAGGED_SLOT = nullptr;
      }
    }
  }
  inline bool window_closeable() noexcept {
    if (char_panel.open || char_bag.open) {
      char_panel.open = false;
      char_bag.open = false;
      return true;
    } else {
      return false;
    }
  }
};
#endif  //MAGE_QUEST_SRC_UI_PLAYER_PLAYERUI_H_
