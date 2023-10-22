#ifndef MAGEQUEST_SRC_GAMEOBJECTS_NPCS_DECKARD_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_NPCS_DECKARD_H_

struct Deckard final : public NPC {
  Deckard(float x, float y, Zone zone)
      : NPC({x, y}, &textures::npc::OLD_MAN, zone, NPC_ID::DECKARD, 1.5) {}
  void Draw() final {
    if (moving) {
      DrawTextureProFastEx(resource->walk[spriteCounter % 78 / 13], pos.x_ + DRAW_X,
                           pos.y_ + DRAW_Y, -23, 0, flip, WHITE);

    } else {
      DrawTextureProFastEx(resource->idle[spriteCounter % 64 / 16], pos.x_ + DRAW_X,
                           pos.y_ + DRAW_Y, -23, 0, flip, WHITE);
    }
    DRAW_HITBOXES();
  }
  void Update() final {
    NPC::Update();
    INTERACT_WITH_PLAYER()
  }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_NPCS_DECKARD_H_
