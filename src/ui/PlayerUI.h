#ifndef MAGE_QUEST_SRC_UI_PLAYER_PLAYERUI_H_
#define MAGE_QUEST_SRC_UI_PLAYER_PLAYERUI_H_

#include "game/MiniMap.h"
#include "game/RegionMap.h"
#include "player/CharacterPanel.h"
#include "player/CharacterBag.h"
#include "player/StatusBar.h"
#include "player/QuestPanel.h"
#include "player/TalentPanel.h"
#include "player/SkillPanel.h"

struct PlayerUI {
  TalentPanel talentPanel;
  RegionMap regionMap{};
  MiniMap miniMap{regionMap.isWindowOpen};
  CharacterPanel charPanel{};
  CharacterBag charBag{};
  StatusBar statusBar{};
  QuestPanel questPanel{};
  HotBar playerHotbar{};
  SkillPanel skillPanel{};
  //Draws the ui elements from lowest to top level visibility
  inline void Draw() noexcept {
    DRAW_NPC_DIALOGUE()
    HealthBar::DrawStatic();
    skillPanel.searchField.UpdateLogic();
    Notifications::Draw();
    playerHotbar.Draw();
    miniMap.Draw();
    statusBar.Draw();
    questPanel.Draw();
    skillPanel.Draw();
    charBag.Draw();
    charPanel.Draw();
    DrawDraggedSlots();
    regionMap.Draw();
    talentPanel.Draw();
  }
  //Order as top windows overlap lower ones //reverse order of drawing
  inline void Update() noexcept {
    WINDOW_FOCUSED = false;
    Notifications::Update();
    talentPanel.Update();
    regionMap.Update();
    miniMap.Update();
    statusBar.Update();
    charPanel.Update();
    charBag.Update();
    skillPanel.Update();
    questPanel.Update();
    playerHotbar.Update();
    UpdateDraggedSlots();
  }
  //When ESC is pressed with open windows they are closed and "consume the action"
  inline bool CloseOpenWindows() noexcept {
    if (charPanel.isWindowOpen || charBag.isWindowOpen || regionMap.isWindowOpen ||
        questPanel.isWindowOpen || talentPanel.isWindowOpen || skillPanel.isWindowOpen) {
      talentPanel.CloseWindow();
      charPanel.CloseWindow();
      charBag.CloseWindow();
      regionMap.CloseWindow();
      questPanel.CloseWindow();
      skillPanel.CloseWindow();
      return true;
    } else {
      return false;
    }
  }
  //Resets the position of the ui elements
  inline void ResetPosition() noexcept {
    charPanel.ResetPosition();
    charBag.ResetPosition();
    questPanel.ResetPosition();
    regionMap.ResetPosition();
  }

 public:
  //Draws all instances of dragged slots or entities
  static inline void DrawDraggedSlots() noexcept {
    auto& ptr = DRAGGED_ITEM;  // thread safety
    if (ptr) {
      ptr->Draw({MOUSE_POS.x - 22, MOUSE_POS.y - 22, 45, 45});
    }

    auto& ptr1 = TOOL_TIP_ITEM;  // thread safety
    if (ptr1) {
      ptr1->DrawToolTip();
      ptr1 = nullptr;
    }

    auto ptr2 = DRAGGED_SKILL_SLOT;  // thread safety
    if (ptr2) {
      ptr2->Draw(MOUSE_POS.x - 25, MOUSE_POS.y - 25);
    }
  }
  //Updates and handles cleanup after unsuccessful drag
  static inline void UpdateDraggedSlots() noexcept {
    if (DRAGGED_ITEM && !IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      if (WINDOW_FOCUSED) {
        ItemSlot::RecoverDraggedItem();
      } else {
        DroppedItem::DropItemAimed(DRAGGED_ITEM);
        DRAGGED_ITEM = nullptr;
        DRAGGED_SLOT = nullptr;
      }
    }
    if (DRAGGED_SKILL_SLOT && IsMouseButtonUp(MOUSE_BUTTON_LEFT)) {
      DRAGGED_SKILL_SLOT = nullptr;
    }
  }
};

#endif  //MAGE_QUEST_SRC_UI_PLAYER_PLAYERUI_H_
