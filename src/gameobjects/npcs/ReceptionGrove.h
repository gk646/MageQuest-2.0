#ifndef MAGEQUEST_SRC_GAMEOBJECTS_NPCS_RECEPTION_GROVE_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_NPCS_RECEPTION_GROVE_H_

struct ReceptionGrove final : public NPC {
  ReceptionGrove(float x, float y, Zone zone)
      : NPC({x, y}, &textures::npc::ARIA, zone, NPC_ID::RECEPTION_GROVE, 1.5) {}
  void Draw() final {
    NPC::Draw();
    if (moving) {
      DrawTextureProFastEx(resource->walk[spriteCounter % 78 / 13], pos.x_ + DRAW_X - 10,
                           pos.y_ + DRAW_Y - 20, -23, 0, isFlipped, WHITE);

    } else {
      DrawTextureProFastEx(resource->idle[spriteCounter % 64 / 16], pos.x_ + DRAW_X - 10,
                           pos.y_ + DRAW_Y - 20, -23, 0, isFlipped, WHITE);
    }

    DRAW_HITBOXES();
  }
  void Update() final {
    NPC::Update();
    INTERACT_WITH_PLAYER()
  }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_NPCS_RECEPTION_GROVE_H_
