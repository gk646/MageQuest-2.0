#ifndef MAGEQUEST_SRC_GAMEOBJECTS_NPCS_NPCS_H
#define MAGEQUEST_SRC_GAMEOBJECTS_NPCS_NPCS_H

struct ReceptionGrove final : public NPC {
  ReceptionGrove(float x, float y, Zone zone)
      : NPC({x, y}, &textures::npc::ARIA, zone, NPC_ID::GROVE_RECEPTION, 1.5) {}
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
struct Satiro final : public NPC {
  Satiro(float x, float y, Zone zone)
      : NPC({x, y}, &textures::npc::SATIRO, zone, NPC_ID::SATIRO, 2) {}
  void Draw() final {
    NPC::Draw();
    if (actionState == 1) {
      DrawSpecial();
    } else {
      if (moving) {
        DrawTextureProFastEx(resource->walk[spriteCounter % 104 / 13],
                             pos.x_ + DRAW_X - 23, pos.y_ + DRAW_Y - 18, 0, 0, isFlipped,
                             WHITE);

      } else {
        DrawTextureProFastEx(resource->idle[spriteCounter % 78 / 13],
                             pos.x_ + DRAW_X - 20, pos.y_ + DRAW_Y - 18, 0, 0, isFlipped,
                             WHITE);
      }
    }

    DRAW_HITBOXES();
  }
  void Update() final {
    NPC::Update();
    INTERACT_WITH_PLAYER()
  }
  inline void DrawSpecial() noexcept {
    int num = spriteCounter % 150 / 15;
    if (num < 9) {
      DrawTextureProFastEx(resource->special[num], pos.x_ + DRAW_X - 20,
                           pos.y_ + DRAW_Y - 18, 0, 0, isFlipped, WHITE);
    } else {
      actionState = 0;
    }
  }
};
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
struct Marla final : public NPC {

  Marla(float x, float y, Zone zone)
      : NPC({x, y}, &textures::npc::MARLA, zone, NPC_ID::MARLA, 2) {}
  void Draw() final {
    NPC::Draw();
    if (moving) {
      DrawTextureProFastEx(resource->walk[spriteCounter % 78 / 13], pos.x_ + DRAW_X,
                           pos.y_ + DRAW_Y, -23, 0, isFlipped, WHITE);

    } else {
      DrawTextureProFastEx(resource->idle[spriteCounter % 64 / 16], pos.x_ + DRAW_X - 3,
                           pos.y_ + DRAW_Y - 20, -23, 0, isFlipped, WHITE);
    }
    DRAW_HITBOXES();
  }
  void Update() final {
    NPC::Update();
    INTERACT_WITH_PLAYER()
  }
};
struct Deckard final : public NPC {
  Deckard(float x, float y, Zone zone)
      : NPC({x, y}, &textures::npc::OLD_MAN, zone, NPC_ID::DECKARD, 1.5) {}
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
    if (spriteCounter > 86) {
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
struct GroveTickets final : public Trader {
  GroveTickets(float x, float y, Zone zone)
      : Trader({x, y}, &textures::npc::ARIA, zone, NPC_ID::ARIA, 2) {}
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
    tradeWindow.Draw();
  }
  void Update() final {
    NPC::Update();
    tradeWindow.Update();
    InteractWithTrader();
  }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_NPCS_NPCS_H
