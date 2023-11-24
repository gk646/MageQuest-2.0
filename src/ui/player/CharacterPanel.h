#ifndef MAGEQUEST_SRC_UI_PLAYER_CHARACTERPANEL_H_
#define MAGEQUEST_SRC_UI_PLAYER_CHARACTERPANEL_H_

#include "elements/CharacterItems.h"
struct CharacterPanel : public Window {
  static constexpr int WIDTH = 350;
  static constexpr int HEIGHT = 450;
  CharacterItems itemsTab;
  CharacterPanel() noexcept
      : Window(SCREEN_WIDTH * 0.1F, SCREEN_HEIGHT * 0.2F, WIDTH, HEIGHT, 18,
               PLAYER_NAME.data(), KEY_C, sound::openInventory, sound::closeInventory) {}
  void Draw() noexcept {
    WINDOW_LOGIC()
    DrawWindow();
    itemsTab.Draw(wholeWindow);
  }
  void Update() noexcept {
    WINDOW_UPDATE();
    itemsTab.Update();
  }

 private:
};

void EntityStats::CheckForItemSets() noexcept {
  int arr[(int)ItemSetNum::END] = {0};
  for (int i = 0; i < 10; i++) {
    const auto item = PLAYER_EQUIPPED[i].item;
    if (!item) continue;
    for (int j = 0; j < (int)ItemSetNum::END; j++) {
      arr[j] = ItemSet::CountEquippedSetItemsOfSet(ITEM_SETS[j]);
    }
  }

  for (int i = 0; i < (int)ItemSetNum::END; i++) {
    auto& setEffect = itemSetEffects[i];
    auto& itemSet = ITEM_SETS[i];

    for (int j = 0; j < 3; j++) {
      if (itemSet.thresholds[j] > 0 && arr[i] >= itemSet.thresholds[j]) {
        if (std::find(UNIQUE_EFFECTS.begin(), UNIQUE_EFFECTS.end(),
                      setEffect.effects[j]) == UNIQUE_EFFECTS.end()) {
          UNIQUE_EFFECTS.push_back(setEffect.effects[j]->OnAdd());
        }
      } else if (setEffect.effects[j] &&
                 std::find(UNIQUE_EFFECTS.begin(), UNIQUE_EFFECTS.end(),
                           setEffect.effects[j]) != UNIQUE_EFFECTS.end()) {
        setEffect.effects[j]->OnRemove();
        UNIQUE_EFFECTS.erase(std::remove(UNIQUE_EFFECTS.begin(), UNIQUE_EFFECTS.end(),
                                         setEffect.effects[j]),
                             UNIQUE_EFFECTS.end());
      }
    }
  }
}
#endif  //MAGEQUEST_SRC_UI_PLAYER_CHARACTERPANEL_H_
