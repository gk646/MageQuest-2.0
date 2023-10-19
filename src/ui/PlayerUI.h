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
  MiniMap mini_map{&region_map.open};
  CharacterPanel char_panel{};
  CharacterBag char_bag{};
  StatusBar status_bar{};
  QuestPanel questPanel{};
  HotBar playerHotbar{};

  void Draw() noexcept {
    DRAW_NPC_DIALOGUE()
    PLAYER_EFFECTS.draw();
    playerHotbar.Draw();
    questPanel.Draw();
    status_bar.draw();
    mini_map.Draw();
    char_panel.draw();
    char_bag.Draw();
    region_map.draw();
    draw_special_items();
  }

  void Update() noexcept {
    WINDOW_FOCUSED = false;
    questPanel.Update();
    mini_map.Update();
    status_bar.update();
    char_panel.update();
    char_bag.Update();
    region_map.Update();
    playerHotbar.Update();
    Util::update();
    update_special_items();
  }
  static inline void draw_special_items() noexcept {
    if (DRAGGED_ITEM) {
      DRAGGED_ITEM->Draw({MOUSE_POS.x - 22, MOUSE_POS.y - 22, 45, 45});
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
        WORLD_OBJECTS.push_back(new DroppedItem(
            {PLAYER_X + PLAYER.size.x_ / 2 + 50, PLAYER_Y + PLAYER.size.y_ / 2},
            DRAGGED_ITEM));
        DRAGGED_ITEM = nullptr;
        DRAGGED_SLOT = nullptr;
      }
    }
  }
  inline bool window_closeable() noexcept {
    if (char_panel.open || char_bag.open || region_map.open) {
      char_panel.open = false;
      char_bag.open = false;
      region_map.open = false;
      return true;
    } else {
      return false;
    }
  }
  inline void ResetPosition() noexcept {
    char_panel.ResetPosition();
    char_bag.ResetPosition();
    questPanel.ResetPosition();
    region_map.ResetPosition();
  }
};

#endif  //MAGE_QUEST_SRC_UI_PLAYER_PLAYERUI_H_
