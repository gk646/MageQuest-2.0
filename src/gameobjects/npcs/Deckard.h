#ifndef MAGEQUEST_SRC_GAMEOBJECTS_NPCS_DECKARD_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_NPCS_DECKARD_H_

struct Deckard final : public NPC {
  Deckard(int x, int y)
      : NPC({x * TILE_SIZE, y * TILE_SIZE}, &textures::OLD_MAN, NPC_ID::DECKARD) {}
  void draw() final {
    draw_dialogue();
    if (moving) {
      DrawTextureProFastEx(resource->walk[sprite_counter % 60 / 10], pos.x_ + DRAW_X,
                           pos.y_ + DRAW_Y, -23, 0, flip, WHITE);

    } else {
      DrawTextureProFastEx(resource->idle[sprite_counter % 60 / 15], pos.x_ + DRAW_X,
                           pos.y_ + DRAW_Y, -23, 0, flip, WHITE);
    }
#ifdef DRAW_HITBOXES
    draw_hitbox();
#endif
  }
  void update() final {
    NPC::update();
    if (IsKeyPressedU::E()) {
      if (this->intersects(PLAYER)) {
        if (!dialogue) {
          PLAYER_QUESTS.interact_npc(this);
        } else if (show_dial_delay < 0) {
          dial_count = 0;
          show_dial_delay = 400;
        } else if (dial_count < 1000) {
          dial_count = 1000;
        } else {
          PLAYER_QUESTS.interact_npc(this);
        }
      }
    }
  }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_NPCS_DECKARD_H_
