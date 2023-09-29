#ifndef MAGE_QUEST_SRC_UI_PLAYER_PLAYERUI_H_
#define MAGE_QUEST_SRC_UI_PLAYER_PLAYERUI_H_

#include "game/MiniMap.h"
#include "game/RegionMap.h"
#include "player/CharacterPanel.h"
#include "player/CharacterBag.h"
#include "player/StatusBar.h"
#include "ui/player/QuestPanel.h"

struct QuestSidePanel final : public Panel {
  static inline char HEADER[] = "Objectives";
  QuestSidePanel() : Panel(200, 150, 16, MINECRAFT_BOLD, HEADER) {}

  void DrawContent() noexcept final{
    if(PLAYER_QUESTS.HaveActiveQuest()){
      DrawOutlineText(MINECRAFT_BOLD, SCALE(15),PLAYER_QUESTS.activeQuest->name.c_str(), body.x+SCALE(2), body.y+SCALE(35), 1, Colors::LightGrey, Colors::black);
      DrawOutlineText(MINECRAFT_BOLD, SCALE(15),PLAYER_QUESTS.activeQuest->GetActiveObjective().c_str(), body.x+SCALE(15),body.y+SCALE(50), 1, Colors::LightGrey, Colors::black);
    }
  }
};

struct QuestPanel final : public Window {
  static inline char HEADER[] = "Quest";
  static constexpr int width = 500;

  QuestSidePanel side_panel;
  QuestPanel()
      : Window(SCREEN_WIDTH * 0.1F, SCREEN_HEIGHT * 0.2F, width, 500, 25, HEADER, KEY_J) {
  }
  void Draw() {
    side_panel.Draw(SCREEN_WIDTH - 230, SCALE(300));
  }
  void Update() {
    side_panel.Update();
    PLAYER_QUESTS.Update();
  }
};

struct PlayerUI {
  RegionMap region_map{};
  MiniMap mini_map{&region_map.open};
  CharacterPanel char_panel{};
  CharacterBag char_bag{};
  StatusBar status_bar{};
  QuestPanel questPanel{};

  void draw() noexcept {
    DRAW_NPC_DIALOGUE()
    PLAYER_EFFECTS.draw();
    PLAYER_HOTBAR.draw();
    questPanel.Draw();
    status_bar.draw();
    mini_map.draw();
    char_panel.draw();
    char_bag.draw();
    region_map.draw();
    draw_special_items();
  }
  void update() noexcept {
    WINDOW_FOCUSED = false;
    questPanel.Update();
    mini_map.update();
    status_bar.update();
    char_panel.update();
    char_bag.update();
    region_map.update();
    IsKeyPressedU::update();
    update_special_items();
  }
  static inline void draw_special_items() noexcept {
    if (DRAGGED_ITEM) {
      DRAGGED_ITEM->draw({MOUSE_POS.x - 22, MOUSE_POS.y - 22, 45, 45});
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
        WORLD_OBJECTS.push_back(new DroppedItem({PLAYER_X+ PLAYER.size.x_/2+50, PLAYER_Y+ PLAYER.size.y_/2}, DRAGGED_ITEM));
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
  inline void ResetPosition() noexcept{
    char_panel.reset_pos();
    char_bag.reset_pos();
    questPanel.reset_pos();
    region_map.reset_pos();
  }
};




#endif  //MAGE_QUEST_SRC_UI_PLAYER_PLAYERUI_H_
