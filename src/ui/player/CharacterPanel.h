#ifndef MAGEQUEST_SRC_UI_PLAYER_CHARACTERPANEL_H_
#define MAGEQUEST_SRC_UI_PLAYER_CHARACTERPANEL_H_

struct CharacterPanel : public Window {

  static constexpr int WIDTH = 350;
  static constexpr int PADDING_LEFT = 22;
  static constexpr int PADDING_TOP = 60;
  static constexpr int PADDING_RIGHT = 290;
  static constexpr int GAP_TOP = 55;
  std::array<TextCell, 18> baseStats =
      TextCell::CreateCharacterCells(WIDTH / 2.5F, 12, MINECRAFT_BOLD, 15);
  std::array<ItemSlot, 10> equipSlots = {
      //Left, from top to bottom
      ItemSlot(PADDING_LEFT, PADDING_TOP, ItemType::HEAD),
      ItemSlot(PADDING_LEFT, PADDING_TOP + GAP_TOP, ItemType::CHEST),
      ItemSlot(PADDING_LEFT, PADDING_TOP + GAP_TOP * 2, ItemType::PANTS),
      ItemSlot(PADDING_LEFT, PADDING_TOP + GAP_TOP * 3, ItemType::BOOTS),

      //Right, from top to bottom
      ItemSlot(PADDING_RIGHT, PADDING_TOP, ItemType::AMULET),
      ItemSlot(PADDING_RIGHT, PADDING_TOP + GAP_TOP, ItemType::RING),
      ItemSlot(PADDING_RIGHT, PADDING_TOP + GAP_TOP * 2, ItemType::RING),
      ItemSlot(PADDING_RIGHT, PADDING_TOP + GAP_TOP * 3, ItemType::RELIC),

      // bottom middle
      ItemSlot(WIDTH / 2 - SLOT_SIZE * 1.5F, PADDING_TOP + GAP_TOP * 6.2F,
               ItemType::ONE_HAND),
      ItemSlot(WIDTH / 2 + SLOT_SIZE / 2, PADDING_TOP + GAP_TOP * 6.2F,
               ItemType::OFF_HAND)};

  TexturedButton spendPoint{14,
                            14,
                            "",
                            0,
                            textures::ui::spendButtonNormal,
                            textures::ui::spendButtonHovered,
                            textures::ui::spendButtonPressed};

  CharacterPanel() noexcept
      : Window(SCREEN_WIDTH * 0.1F, SCREEN_HEIGHT * 0.2F, WIDTH, 450, 18,
               PLAYER_NAME.data(), KEY_C, sound::openInventory, sound::closeInventory) {
    PLAYER_EQUIPPED = equipSlots.data();
  }
  void Draw() noexcept {
    WINDOW_LOGIC()
    DrawWindow();
    RectangleR scaleWhole = SCALE_RECT(wholeWindow);
    float x = scaleWhole.x + SCALE(PADDING_LEFT / 2.0F);
    float y = scaleWhole.y + SCALE(275);
    DrawStatCells(x, y);
    DrawHeaderText(scaleWhole.x, scaleWhole.y, scaleWhole.width);
    DrawBigPlayer(scaleWhole);
    for (auto& slot : equipSlots) {
      slot.DrawCharacterSlot(wholeWindow.x, wholeWindow.y);
      slot.DrawBackGroundIcons();
    }
  }
  void Update() noexcept {
    WINDOW_UPDATE();
    for (auto& slot : equipSlots) {
      slot.UpdateCharacterSlots();
    }
  }

 private:
  static void DrawBigPlayer(const RectangleR& scaledRect) noexcept {
    int sprite_counter = PLAYER.spriteCounter;
    Point pos{scaledRect.x + SCALE(50), scaledRect.y - SCALE(30)};
    auto flip = PLAYER.flip;
    int action_state = PLAYER.actionState;
    auto resource = PLAYER.resource;
    float height = scaledRect.height / 1.4F;
    if (PLAYER.moving) {
      DrawTextureScaled(resource->walk[sprite_counter % 64 / 8],
                        {pos.x_ - 80, pos.y_ - 45, scaledRect.width, height}, 0, flip, 55,
                        WHITE);
    } else if (action_state == 1) {
      int num = sprite_counter % 48 / 6;
      if (num < 7) {
        DrawTextureScaled(resource->attack1[num],
                          {pos.x_ - 75, pos.y_ - 45, scaledRect.width, height}, 0, flip,
                          50, WHITE);
      }
    } else if (action_state == 2) {
      int num = sprite_counter % 50 / 5;
      if (num < 9) {
        DrawTextureScaled(resource->attack2[num],
                          {pos.x_ - 25, pos.y_ - 45, scaledRect.width, height}, 0, flip,
                          -10, WHITE);
      }
    } else if (action_state == 3) {
      int num = sprite_counter % 85 / 5;
      if (num < 16) {
        DrawTextureScaled(resource->attack3[num],
                          {pos.x_ - 25, pos.y_ - 45, scaledRect.width, height}, 0, flip,
                          -15, WHITE);
      }
    } else if (action_state == -100) {
      int num = sprite_counter % 75 / 15;
      if (num < 4) {
        DrawTextureScaled(resource->death[num],
                          {pos.x_ - 25, pos.y_ - 45, scaledRect.width, height}, 0, flip,
                          -22, WHITE);
      }
    }
    if (!PLAYER.moving && action_state == 0) {
      DrawTextureScaled(resource->idle[sprite_counter % 80 / 10],
                        {pos.x_ - 60, pos.y_ - 45, scaledRect.width, height}, 0, flip, 20,
                        WHITE);
    }
    DrawRectangleRoundedLines(
        {scaledRect.x + SCALE(PADDING_LEFT * 3.2F), scaledRect.y + SCALE(PADDING_TOP),
         scaledRect.width - SCALE(PADDING_LEFT * 6.4F), scaledRect.height - SCALE(240)},
        0.1F, ROUND_SEGMENTS, 2, Colors::darkBackground);
  }
  static void DrawHeaderText(float x, float y, float size) noexcept {
    if (PLAYER_SECOND_STATS.HasAttributePointsToSpend()) {
      snprintf(TEXT_BUFFER, TEXT_BUFFER_SIZE, "Unspent Attribute Points!: %i",
               PLAYER_SECOND_STATS.attributePointsToSpend);
      Util::DrawCenteredText(MINECRAFT_BOLD, SCALE(15), TEXT_BUFFER, x + size / 2,
                             y + SCALE(40), Colors::darkBackground);
    }
    snprintf(TEXT_BUFFER, TEXT_BUFFER_SIZE, "Level: %i", (int)PLAYER_STATS.level);
    Util::DrawCenteredText(MINECRAFT_BOLD, SCALE(16), TEXT_BUFFER, x + size / 2,
                           y + SCALE(25), Colors::darkBackground);
  }
  inline void DrawStatCells(float x, float y) noexcept {
    x += baseStats[0].bounds.width + SCALE(22);
    int i = 9;
    DrawSingleStatCell(MAX_HEALTH, x, y, i, PLAYER_STATS.GetMaxHealth());
    DrawSingleStatCell(MAX_MANA, x, y, i, PLAYER_STATS.GetMaxMana());
    DrawSingleStatCell(HEALTH_REGEN, x, y, i, PLAYER_STATS.GetHealthRegen());
    DrawSingleStatCell(MANA_REGEN, x, y, i, PLAYER_STATS.GetManaRegen());
    DrawSingleStatCell(ARMOUR, x, y, i, PLAYER_STATS.GetArmour());
    DrawSingleStatCell(CRIT_CHANCE, x, y, i, PLAYER_STATS.effects[CRIT_CHANCE]);
    DrawSingleStatCell(DODGE_CHANCE, x, y, i, PLAYER_STATS.effects[DODGE_CHANCE]);
    DrawSingleStatCell(SPEED_MULT_P, x, y, i, 1 + PLAYER_STATS.effects[SPEED_MULT_P]);
    DrawSingleStatCell(WEAPON_DAMAGE, x, y, i, PLAYER_STATS.effects[WEAPON_DAMAGE]);

    y -= 9 * (baseStats[0].bounds.height + 1);
    x -= baseStats[0].bounds.width + SCALE(22);
    for (int j = 0; j < 9; j++) {
      Stat stat = Stat(j);
      if (PLAYER_SECOND_STATS.SpendAttributePoint(i) &&
          spendPoint.Draw(x + WIDTH / 2.5F + SCALE(10), y)) {
        PLAYER_STATS.SpendAttributePoint(j);
      }
      snprintf(TEXT_BUFFER, TEXT_BUFFER_SIZE, "%s:", statToName[stat].c_str());
      baseStats[j].DrawStatCell(
          x, y, TEXT_BUFFER, (int)std::round(PLAYER_STATS.effects[j]),
          PLAYER_SECOND_STATS.IsDefaultValue(stat) ? Colors::darkBackground
                                                   : Colors::StatGreen);
      y += baseStats[j].bounds.height + 1;
    }
  }
  inline void DrawSingleStatCell(Stat stat, float x, float& y, int& i,
                                 float val) noexcept {
    snprintf(TEXT_BUFFER, TEXT_BUFFER_SIZE, "%s:", statToName[stat].c_str());
    if (stat == WEAPON_DAMAGE) {
      baseStats[i++].DrawStatCell(x, y, TEXT_BUFFER, val, Alignment::RIGHT,
                                  Colors::darkBackground, Colors::darkBackground, "%.2f");
    } else {
      baseStats[i++].DrawStatCell(x, y, TEXT_BUFFER, val);
    }
    y += baseStats[0].bounds.height + 1;
  }
};

void EntityStats::CheckForItemSets() noexcept {
  int arr[(int)ItemSetNum::END] = {0};
  for (int i = 0; i < 10; i++) {
    const auto item = PLAYER_EQUIPPED[i].item;
    if (!item) continue;
    for (int j = 0; j < (int)ItemSetNum::END; j++) {
      if (ItemSet::IsPartOfItemSet(item, ITEM_SETS[j])) arr[j]++;
    }
  }

  for (int i = 0; i < (int)ItemSetNum::END; i++) {
    auto& setEffect = itemSetEffects[i];
    auto& itemSet = ITEM_SETS[i];

    for (int j = 0; j < 3; j++) {
      if (itemSet.thresholds[i] > 0 && arr[i] >= itemSet.thresholds[i]) {
        if (std::find(UNIQUE_EFFECTS.begin(), UNIQUE_EFFECTS.end(),
                      setEffect.effects[i]) == UNIQUE_EFFECTS.end()) {
          UNIQUE_EFFECTS.push_back(setEffect.effects[i]->OnAdd());
        }
      } else if (setEffect.effects[i] &&
                 std::find(UNIQUE_EFFECTS.begin(), UNIQUE_EFFECTS.end(),
                           setEffect.effects[i]) != UNIQUE_EFFECTS.end()) {
        setEffect.effects[i]->OnRemove();
        UNIQUE_EFFECTS.erase(std::remove(UNIQUE_EFFECTS.begin(), UNIQUE_EFFECTS.end(),
                                         setEffect.effects[i]),
                             UNIQUE_EFFECTS.end());
      }
    }
  }
}
#endif  //MAGEQUEST_SRC_UI_PLAYER_CHARACTERPANEL_H_
