#ifndef MAGEQUEST_SRC_GAMEOBJECTS_NPCS_ARIA_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_NPCS_ARIA_H_

struct Aria final : public NPC {
  Aria(float x, float y, Zone zone)
      : NPC({x, y}, &textures::npc::ARIA, zone, NPC_ID::ARIA, 2) {}
  void Draw() final {
    NPC::Draw();
    if (actionState == -100) [[unlikely]] {
      DrawDeath();
    } else if (actionState == 1) {
      DrawAttack1();
    } else if (actionState == 2) {
      DrawAttack2();
    } else {
      if (moving) {
        DrawTextureProFastEx(resource->walk[spriteCounter % 78 / 13],
                             pos.x_ + DRAW_X - 10, pos.y_ + DRAW_Y - 20, -23, 0,
                             isFlipped, WHITE);

      } else {
        DrawTextureProFastEx(resource->idle[spriteCounter % 64 / 16],
                             pos.x_ + DRAW_X - 10, pos.y_ + DRAW_Y - 20, -23, 0,
                             isFlipped, WHITE);
      }
    }
    DRAW_HITBOXES();
  }
  void Update() final {
    NPC::Update();
    INTERACT_WITH_PLAYER()
  }
  inline void DrawDeath() noexcept {
    int num = spriteCounter % 91 / 15;
    if(spriteCounter> 86){
      spriteCounter = 86;
    }
    if (num < 6) {
      DrawTextureProFastEx(resource->death[num], pos.x_ + DRAW_X - 8,
                           pos.y_ + DRAW_Y - 20, 3, 0, !isFlipped, WHITE);
    }
  }
  inline void DrawAttack1() noexcept {
    int num = spriteCounter % 73 / 12;
    if (num < 6) {
      DrawTextureProFastEx(resource->attack1[num], pos.x_ + DRAW_X - 10,
                           pos.y_ + DRAW_Y - 20, 10, 0, !isFlipped, WHITE);
    } else {
      actionState = 0;
    }
  }
  inline void DrawAttack2() noexcept {
    int num = spriteCounter % 73 / 12;
    if (num < 6) {
      DrawTextureProFastEx(resource->attack2[num], pos.x_ + DRAW_X - 9,
                           pos.y_ + DRAW_Y - 19, 10, 0, !isFlipped, WHITE);
    } else {
      actionState = 0;
    }
  }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_NPCS_ARIA_H_
