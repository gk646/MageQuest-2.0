#ifndef MAGEQUEST_SRC_GAMEOBJECTS_NPCS_NIETZSCHE_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_NPCS_NIETZSCHE_H_

struct Nietzsche final : public NPC {
  Nietzsche(float x, float y, Zone zone)
      : NPC({x, y}, &textures::npc::NIETZSCHE, zone, NPC_ID::NIETZSCHE, 1.5) {}
  void Draw() final {
    if (moving) {
      DrawTextureProFastEx(resource->walk[spriteCounter % 78 / 13], pos.x_ + DRAW_X,
                           pos.y_ + DRAW_Y, -23, 0, isFlipped, WHITE);

    } else {
      DrawTextureProFastEx(resource->idle[spriteCounter % 64 / 16], pos.x_ + DRAW_X,
                           pos.y_ + DRAW_Y, -23, 0, isFlipped, WHITE);
    }
    DRAW_HITBOXES();
  }
  void Update() final {
    NPC::Update();
    INTERACT_WITH_PLAYER()
  }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_NPCS_NIETZSCHE_H_
