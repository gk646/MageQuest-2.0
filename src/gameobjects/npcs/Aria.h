#ifndef MAGEQUEST_SRC_GAMEOBJECTS_NPCS_ARIA_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_NPCS_ARIA_H_

struct Aria final : public NPC {

  Aria(float x, float y, Zone zone) :  NPC({x, y}, &textures::npc::ARIA, zone, NPC_ID::ARIA, 2) {}
  void draw() final {
    if (moving) {
      DrawTextureProFastEx(resource->walk[sprite_counter % 78 / 13], pos.x_ + DRAW_X,
                           pos.y_ + DRAW_Y, -23, 0, flip, WHITE);

    } else {
      DrawTextureProFastEx(resource->idle[sprite_counter % 64 / 16], pos.x_ + DRAW_X-3,
                           pos.y_ + DRAW_Y-20, -23, 0, flip, WHITE);
    }
#ifdef DRAW_HITBOXES
    draw_hitbox();
#endif
  }
  void Update() final {
    NPC::Update();
    INTERACT_WITH_PLAYER()
  }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_NPCS_ARIA_H_
