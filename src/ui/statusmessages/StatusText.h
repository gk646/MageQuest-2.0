#ifndef MAGEQUEST_SRC_UI_STATUSMESSAGES_STATUSTEXT_H_
#define MAGEQUEST_SRC_UI_STATUSMESSAGES_STATUSTEXT_H_

namespace StatusText {
struct ItemPickup {
  Item* item = nullptr;
  float offSet = 5;
  inline void Draw() const noexcept {
    if (!item) return;
    snprintf(TEXT_BUFFER, TEXT_BUFFER_SIZE, "+1 %s", item->name.c_str());

    float textWidth = MeasureTextEx(MINECRAFT_BOLD, TEXT_BUFFER, 15, 0.5F).x;
    DrawTextExR(MINECRAFT_BOLD, TEXT_BUFFER,
                {CAMERA_X - textWidth / 2.0F, CAMERA_Y - (float)offSet}, 15, 0.5F,
                rarity_to_color[item->rarity]);

    DrawTextureScaled(
        item->texture,
        {CAMERA_X + textWidth / 2.0F + 5, CAMERA_Y - (float)offSet -2, 20, 20}, 0,
        false, 0, WHITE);
  }
  inline void Reset(Item* newItem) {
    item = newItem;
    offSet = 10;
  }
  inline void Update() {
    if (!item) return;
    offSet += 0.5F;
    if (offSet >= 110.0F) {
      offSet = 5;
      item = nullptr;
    }
  }
};
inline static ItemPickup lastItems[5] = {};

inline static void AddItemPickup(Item* item) {
  for (auto& lastItem : lastItems) {
    if (!lastItem.item) {
      lastItem.Reset(item);
      return;
    }
  }
  lastItems[0].Reset(item);
}

static void Draw() noexcept {
  for (int i = 4; i > -1; i--) {
    lastItems[i].Draw();
  }
}

inline static void Update() noexcept {
  for (auto& pickup : lastItems) {
    pickup.Update();
  }
}
}  // namespace StatusText
#endif  //MAGEQUEST_SRC_UI_STATUSMESSAGES_STATUSTEXT_H_
