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
  std::array<InventorySlot, 10> equipSlots = {
      //Left, from top to bottom
      InventorySlot(PADDING_LEFT, PADDING_TOP, ItemType::HEAD),
      InventorySlot(PADDING_LEFT, PADDING_TOP + GAP_TOP, ItemType::CHEST),
      InventorySlot(PADDING_LEFT, PADDING_TOP + GAP_TOP * 2, ItemType::PANTS),
      InventorySlot(PADDING_LEFT, PADDING_TOP + GAP_TOP * 3, ItemType::BOOTS),

      //Right, from top to bottom
      InventorySlot(PADDING_RIGHT, PADDING_TOP, ItemType::AMULET),
      InventorySlot(PADDING_RIGHT, PADDING_TOP + GAP_TOP, ItemType::RING),
      InventorySlot(PADDING_RIGHT, PADDING_TOP + GAP_TOP * 2, ItemType::RING),
      InventorySlot(PADDING_RIGHT, PADDING_TOP + GAP_TOP * 3, ItemType::RELIC),

      // bottom middle
      InventorySlot(WIDTH / 2 - SLOT_SIZE * 1.5F, PADDING_TOP + GAP_TOP * 6.2F,
                    ItemType::ONE_HAND),
      InventorySlot(WIDTH / 2 + SLOT_SIZE / 2, PADDING_TOP + GAP_TOP * 6.2F,
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
  void Draw() {
    WINDOW_LOGIC()
    DrawWindow();
    RectangleR scaleWhole = SCALE_RECT(wholeWindow);
    float x = scaleWhole.x + SCALE(PADDING_LEFT / 2.0F);
    float y = scaleWhole.y + SCALE(275);
    DrawStatCells(x, y);
    DrawHeaderText(scaleWhole.x, scaleWhole.y, scaleWhole.width);
    DrawPlayer(scaleWhole);
    for (auto& slot : equipSlots) {
      slot.DrawCharacterSlot(wholeWindow.x, wholeWindow.y);
      slot.DrawBackGroundIcons();
    }
  }
  static void DrawPlayer(const RectangleR& scaled_rect) noexcept {
    int sprite_counter = PLAYER.spriteCounter;
    Point pos{scaled_rect.x + SCALE(50), scaled_rect.y - SCALE(30)};
    auto flip = PLAYER.flip;
    int action_state = PLAYER.actionState;
    auto resource = PLAYER.resource;
    float height = scaled_rect.height / 1.4F;
    if (PLAYER.moving) {
      DrawTextureScaled(resource->walk[sprite_counter % 64 / 8],
                        {pos.x_ - 80, pos.y_ - 45, scaled_rect.width, height}, 0, flip,
                        55, WHITE);
    } else if (action_state == 1) {
      int num = sprite_counter % 48 / 6;
      if (num < 7) {
        DrawTextureScaled(resource->attack1[num],
                          {pos.x_ - 75, pos.y_ - 45, scaled_rect.width, height}, 0, flip,
                          50, WHITE);
      }
    } else if (action_state == 2) {
      int num = sprite_counter % 50 / 5;
      if (num < 9) {
        DrawTextureScaled(resource->attack2[num],
                          {pos.x_ - 25, pos.y_ - 45, scaled_rect.width, height}, 0, flip,
                          -10, WHITE);
      }
    } else if (action_state == 3) {
      int num = sprite_counter % 85 / 5;
      if (num < 16) {
        DrawTextureScaled(resource->attack3[num],
                          {pos.x_ - 25, pos.y_ - 45, scaled_rect.width, height}, 0, flip,
                          -15, WHITE);
      }
    } else if (action_state == -100) {
      int num = sprite_counter % 75 / 15;
      if (num < 4) {
        DrawTextureScaled(resource->death[num],
                          {pos.x_ - 25, pos.y_ - 45, scaled_rect.width, height}, 0, flip,
                          -22, WHITE);
      }
    }
    if (!PLAYER.moving && action_state == 0) {
      DrawTextureScaled(resource->idle[sprite_counter % 80 / 10],
                        {pos.x_ - 60, pos.y_ - 45, scaled_rect.width, height}, 0, flip,
                        20, WHITE);
    }
    DrawRectangleRoundedLines(
        {scaled_rect.x + SCALE(PADDING_LEFT * 3.2F), scaled_rect.y + SCALE(PADDING_TOP),
         scaled_rect.width - SCALE(PADDING_LEFT * 6.4F), scaled_rect.height - SCALE(240)},
        0.1F, ROUND_SEGMENTS, 2, Colors::darkBackground);
  }
  static void DrawHeaderText(float x, float y, float size) noexcept {
    if (PLAYER_SPENT_POINTS.HasPointsToSpend()) {
      snprintf(TEXT_BUFFER, TEXT_BUFFER_SIZE, "Unspent Attribute Points!: %i",
                PLAYER_SPENT_POINTS.pointsToSpend);
      Util::DrawCenteredText(MINECRAFT_BOLD, SCALE(15), TEXT_BUFFER, x + size / 2,
                             y + SCALE(40), Colors::darkBackground);
    }
    snprintf(TEXT_BUFFER, TEXT_BUFFER_SIZE, "Level: %i", (int)PLAYER_STATS.level);
    Util::DrawCenteredText(MINECRAFT_BOLD, SCALE(16), TEXT_BUFFER, x + size / 2,
                           y + SCALE(25), Colors::darkBackground);
  }
  void DrawStatCells(float x, float y) noexcept {
    x += baseStats[0].bounds.width + SCALE(22);
    int i = 9;
    DrawSingleStatCell(MAX_HEALTH, x, y, i);
    DrawSingleStatCell(MAX_MANA, x, y, i);
    DrawSingleStatCell(HEALTH_REGEN, x, y, i);
    DrawSingleStatCell(MANA_REGEN, x, y, i);
    DrawSingleStatCell(ARMOUR, x, y, i);
    DrawSingleStatCell(CRIT_CHANCE, x, y, i);
    DrawSingleStatCell(DODGE_CHANCE, x, y, i);
    DrawSingleStatCell(SPEED_MULT_P, x, y, i);
    DrawSingleStatCell(WEAPON_DAMAGE, x, y, i);

    y -= 9 * (baseStats[0].bounds.height + 1);
    x -= baseStats[0].bounds.width + SCALE(22);
    for (uint_fast32_t j = 0; j < 9; j++) {
      Stat stat = Stat(j);
      if (PLAYER_SPENT_POINTS.HasPointsToSpend() &&
          spendPoint.Draw(x + WIDTH / 2.5F + SCALE(10), y)) {
        PLAYER_STATS.SpendAttributePoint(j);
      }
      snprintf(TEXT_BUFFER, TEXT_BUFFER_SIZE, "%s:", statToName[stat].c_str());
      baseStats[j].DrawStatCell(x, y, TEXT_BUFFER, (int)PLAYER_STATS.effects[j],
                                PLAYER_SPENT_POINTS.IsDefaultValue(stat)
                                    ? Colors::darkBackground
                                    : Colors::StatGreen);

      y += baseStats[j].bounds.height + 1;
    }
  }
  void update() noexcept {
    WINDOW_UPDATE();
    for (auto& slot : equipSlots) {
      slot.UpdateCharacterSlots();
    }
  }

 private:
  inline void DrawSingleStatCell(Stat stat, float x, float& y, int& i) noexcept {
    snprintf(TEXT_BUFFER, TEXT_BUFFER_SIZE, "%s:", statToName[stat].c_str());
    if (i == 16) {
      baseStats[i++].DrawStatCell(x, y, TEXT_BUFFER, 1 + PLAYER_STATS.effects[stat]);
    } else {
      baseStats[i++].DrawStatCell(x, y, TEXT_BUFFER, PLAYER_STATS.effects[stat]);
    }
    y += baseStats[0].bounds.height + 1;
  }
};
#endif  //MAGEQUEST_SRC_UI_PLAYER_CHARACTERPANEL_H_
