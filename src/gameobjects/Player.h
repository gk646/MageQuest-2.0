#ifndef MAGE_QUEST_SRC_ENTITIES_PLAYER_H_
#define MAGE_QUEST_SRC_ENTITIES_PLAYER_H_
//The player character class
struct Player final : public Entity {
  std::string name;
  MonsterResource* resource = &textures::PLAYER_RESOURCE;
  float playerSpriteCount = 0;
  int actionState = 0;
  bool flip = false;
  bool moving = false;
  uint8_t uncoverRadius = 8;
  explicit Player(const Point& pos) : Entity(pos, {28, 48}, ShapeType::RECT) {
    PLAYER_EFFECTS.self = this;
    PLAYER_TILE = &tilePos;
  }
  Player(const Player& other) noexcept : Entity(other) {}
  Player& operator=(const Player& other) {
    if (this == &other) {
      return *this;
    }
    Entity::operator=(other);
    return *this;
  }

 public:
  //Called on each hit
  inline void Hit(Projectile& p) const noexcept {
    if (!p.isFriendlyToPlayer && p.IsActive() && actionState != -100) {
      ApplyTalentsToPlayerHit(&p);
      PLAYER_EFFECTS.AddEffects(p.statusEffects);
      PLAYER_STATS.TakeDamage(p.damageStats, this);
      p.isDead = p.hitType == HitType::ONE_HIT;
    }
  }
  void Draw() final {
    if (moving) {
      DrawTextureProFastEx(resource->walk[(int)playerSpriteCount % 56 / 7],
                           std::floor(pos.x_ + DRAW_X - 25.0F),
                           std::floor(pos.y_ + DRAW_Y - 46), -20, 0, flip, WHITE);
      actionState = 0;
    } else if (actionState == 1) {
      draw_attack1();
    } else if (actionState == 2) {
      draw_attack2();
    } else if (actionState == 3) {
      draw_attack3();
    } else if (actionState == -100) {
      draw_death();
    }
    if (!moving && actionState == 0) {
      DrawTextureProFastEx(resource->idle[spriteCounter % 80 / 10],
                           pos.x_ + DRAW_X - 32.0F, pos.y_ + DRAW_Y - 46, -7, 0, flip,
                           WHITE);
    }
    DRAW_HITBOXES();
  }
  void Update() final {
    //TODO cleanup breakdown into private methods
    playerSpriteCount += 1 * (1 + PLAYER_STATS.effects[SPEED_MULT_P]);
    spriteCounter++;
    if (PLAYER_STATS.health <= 0) {
      GAME_STATE = GameState::GameOver;
      return;
    } else if (PLAYER_STATS.stunned || GAME_STATE == GameState::GameMenu) {
      return;
    }

    float speed = PLAYER_STATS.GetSpeed();

    bool verticalMove = (IsKeyDown(KEY_W) || IsKeyDown(KEY_S));
    bool horizontalMove = (IsKeyDown(KEY_A) || IsKeyDown(KEY_D));

    if (verticalMove && horizontalMove) {
      speed /= SQRT_2;
    }
    moving = verticalMove || horizontalMove;
    if (IsKeyDown(KEY_W) && !tile_collision_up(speed)) {
      pos.y_ -= speed;
      GAME_STATISTICS.WalkPixels(speed);
    }
    if (IsKeyDown(KEY_S) && !tile_collision_down(speed)) {
      pos.y_ += speed;
      GAME_STATISTICS.WalkPixels(speed);
    }
    if (IsKeyDown(KEY_A) && !TileCollisionLeft(speed)) {
      pos.x_ -= speed;
      GAME_STATISTICS.WalkPixels(speed);
      flip = true;
    }
    if (IsKeyDown(KEY_D) && !TileCollisionRight(speed)) {
      pos.x_ += speed;
      GAME_STATISTICS.WalkPixels(speed);
      flip = false;
    }
    if (moving) {
      int num = (int)playerSpriteCount % 56;
      if (num == 14 || num == 42) {
        PlaySoundR(sound::player::grassWalk[RANGE_01(RNG_ENGINE) * 4]);
      }
    }
    tilePos.x = (pos.x_ + size.x / 2) / TILE_SIZE;
    tilePos.y = (pos.y_ + size.y / 2) / TILE_SIZE;

    UncoverMapCover();
    Multiplayer::UDP_SEND_POSITION(static_cast<int16_t>(pos.x_),
                                   static_cast<int16_t>(pos.y_));
  }

 private:
  inline void draw_death() noexcept {
    int num = spriteCounter % 75 / 15;
    if (num < 4) {
      DrawTextureProFastEx(resource->death[num], pos.x_ + DRAW_X - 25,
                           pos.y_ + DRAW_Y - 45, -22, 0, flip, WHITE);
    } else {
      isDead = true;
    }
  }
  inline void draw_attack1() noexcept {
    int num = spriteCounter % 48 / 6;
    if (num < 7) {
      DrawTextureProFastEx(resource->attack1[num], pos.x_ + DRAW_X - 25,
                           pos.y_ + DRAW_Y - 45, -22, 0, flip, WHITE);
    } else {
      actionState = 0;
    }
  }
  inline void draw_attack2() noexcept {
    int num = spriteCounter % 50 / 5;
    if (num < 9) {
      DrawTextureProFastEx(resource->attack2[num], pos.x_ + DRAW_X - 25,
                           pos.y_ + DRAW_Y - 45, -20, 0, flip, WHITE);
    } else {
      actionState = 0;
    }
  }
  inline void draw_attack3() noexcept {
    if (!Skill::lastCastedSkill) {
      actionState = 0;
      Draw();
      return;
    }
    int num = spriteCounter % Skill::lastCastedSkill->skillStats.castTime /
              (Skill::lastCastedSkill->skillStats.castTime / 13.0F);
    if (num < 16) {
      DrawTextureProFastEx(resource->attack3[num], pos.x_ + DRAW_X - 25,
                           pos.y_ + DRAW_Y - 45, -15, 0, flip, WHITE);
    } else {
      actionState = 0;
    }
  }
  inline void UncoverMapCover() noexcept {
    int sx = tilePos.x;
    int sy = tilePos.y;
    int radiusSqrt = uncoverRadius * uncoverRadius;
    int xMin = std::max(sx - uncoverRadius, 0);
    int xMax = std::min(sx + uncoverRadius, CURRENT_MAP_SIZE);
    int yMin = std::max(sy - uncoverRadius, 0);
    int yMax = std::min(sy + uncoverRadius, CURRENT_MAP_SIZE);

    for (int x = xMin; x < xMax; ++x) {
      for (int y = yMin; y < yMax; ++y) {
        if (RANGE_01(RNG_ENGINE) > 0.92F &&
            (x - sx) * (x - sx) + (y - sy) * (y - sy) <= radiusSqrt) {
          CURRENT_MAP_COVER[x][y] = false;
        }
      }
    }
  }
};
inline static Player PLAYER({150, 150});

#include "../ui/player/elements/SkillSlot.h"
#include "../ui/player/HotBar.h"
#include "WorldObject.h"

void EntityStats::ReCalculatePlayerStats() noexcept {
  //TODO remove all item and talent effect before
  PLAYER_EFFECTS.RemoveEffects();
  //Using "=" here means all these stats cant receive flat bonuses unless they are all removed and reapplied
  effects[MAX_HEALTH] = 20.0F + effects[VITALITY] * 5.0F;
  effects[MAX_MANA] = 10.0F + effects[INTELLIGENCE] * 7.5F;

  effects[HEALTH_REGEN] = 0.2F + effects[ENDURANCE] / 18.0F;
  effects[MANA_REGEN] = 1 + effects[WISDOM] / 9.0F;

  effects[SPEED_MULT_P] = (effects[AGILITY] / (level * 2)) / 10;
  effects[DODGE_CHANCE] = (effects[AGILITY] / level) * std::sqrt(level);
  PLAYER_EFFECTS.ApplyEffects();
}
//Returns true if the player is close to the spawn trigger
bool SpawnTrigger::IsClose() const noexcept {
  return PLAYER.pos.dist(pos.x + size.x / 2, pos.y + size.y / 2) < UPDATE_DISTANCE * 48;
}

//Skill implementations using the global "PLAYER" instance
void Skill::UpdateStaticState() noexcept {
  if (Skill::globalCooldown < GLOBAL_COOLDOWN_TICKS) {
    globalCooldown++;
  }
  if (Skill::castProgress >= 0) {
    Skill::castProgress++;
    PLAYER.flip = MOUSE_POS.x < CAMERA_X;
    if (PLAYER.moving) {
      ResetCast();
      return;
    }
    if (Skill::castProgress == Skill::lastCastedSkill->skillStats.castTime) {
      Skill::lastCastedSkill->Activate(false);
    }
  }
}
void Skill::UseResources(bool isFree) noexcept {
  coolDownUpCounter = 0;
  globalCooldown = 0;
  ResetCast();
  PLAYER.flip = MOUSE_POS.x < CAMERA_X;
  PLAYER.spriteCounter = 0;
  if (skillStats.castTime == 0) {
    PLAYER.actionState = attackAnimation;
  }
  if (isFree) return;
  PLAYER_STATS.ApplySkillCosts(skillStats);
}
bool Skill::HandleCasting(bool isFree) noexcept {
  if (skillStats.castTime == 0 || skillStats.castTime == castProgress || isFree) {
    return true;
  }
  castProgress = 0;
  PLAYER.actionState = 3;
  PLAYER.spriteCounter = 0;
  PlaySoundR(sound::player::abilityCast);
  lastCastedSkill = const_cast<Skill*>(this);
  return false;
}
bool Skill::RangeLineOfSightCheck(const Point& targetPos) const noexcept {
  if (Point(PLAYER.GetMiddlePoint()).dist(targetPos) <= (float)skillStats.range) {
    if (PathFinding::LineOfSightCheck(PLAYER.tilePos, targetPos)) {
      return true;
      //TODO quick notifications
    } else {
      // No line of sight to target
    }
  } else {
    // Target is out of range
  }
  return false;
}
void Skill::SkillAtMouse(ProjectileType type, bool isFree) noexcept {
  Point targetPos = {
      PLAYER_X + MOUSE_POS.x - CAMERA_X - typeToInfo[skillStats.type].size.x / 2.0F,
      PLAYER_Y + MOUSE_POS.y - CAMERA_Y - typeToInfo[skillStats.type].size.y / 2.0F};
  if (!RangeLineOfSightCheck(targetPos)) return;
  if (!HandleCasting(isFree)) return;
  UseResources(isFree);
  float damage = GetSkillDamage(type);
  auto prj = GetProjectileInstance(type, targetPos, true, damage, &PLAYER, {0, 0}, {});
  SetDamageStats(prj, PLAYER_STATS.effects[CRIT_CHANCE],
                 PLAYER_STATS.effects[CRIT_DAMAGE_P]);
  PROJECTILES.emplace_back(prj);
  Multiplayer::UDP_SEND_PROJECTILE(type, (int16_t)targetPos.x_, (int16_t)targetPos.y_, 0,
                                   0, 0, damage);
  ApplyTalentsToCast(this);
}
void Skill::SkillAtMouseRadial(ProjectileType type, int numProjectiles,
                               bool isFree) noexcept {
  Point targetPos = {
      PLAYER_X + MOUSE_POS.x - CAMERA_X - typeToInfo[skillStats.type].size.x / 2.0F,
      PLAYER_Y + MOUSE_POS.y - CAMERA_Y - typeToInfo[skillStats.type].size.y / 2.0F};
  if (!RangeLineOfSightCheck(targetPos)) return;
  if (!HandleCasting(isFree)) return;
  UseResources(isFree);
  const float interval_angle = 360.0f / numProjectiles;
  float damage = GetSkillDamage(type);
  for (int_fast32_t i = 0; i < numProjectiles; i++) {
    float angle_rad = interval_angle * i * DEG2RAD;
    float x_component = std::cos(angle_rad);
    float y_component = std::sin(angle_rad);
    float pov = angle_rad * RAD2DEG;

    auto prj = GetProjectileInstance(
        type, targetPos, true, damage, &PLAYER, {x_component, y_component}, pov, {},
        i == numProjectiles - 1 ? sound::fireBurst : sound::EMPTY_SOUND);
    SetDamageStats(prj, PLAYER_STATS.effects[CRIT_CHANCE],
                   PLAYER_STATS.effects[CRIT_DAMAGE_P]);
    PROJECTILES.emplace_back(prj);
    Multiplayer::UDP_SEND_PROJECTILE(type, (int16_t)targetPos.x_, (int16_t)targetPos.y_,
                                     0, 0, 0, damage);
  }
  ApplyTalentsToCast(this);
}
void Skill::SkillToMouse(ProjectileType type, bool isFree) noexcept {
  if (!HandleCasting(isFree)) return;
  UseResources(isFree);
  Point targetPos = {
      PLAYER_X + (float)PLAYER.size.x / 2.0F,
      PLAYER_Y +
          ((float)PLAYER.size.y - (float)typeToInfo[skillStats.type].size.y) / 2.0F};
  float angle = std::atan2(
      MOUSE_POS.y - targetPos.y_ - DRAW_Y - typeToInfo[skillStats.type].size.y / 2,
      MOUSE_POS.x - targetPos.x_ - DRAW_X - typeToInfo[skillStats.type].size.x / 2);
  float pov = angle * (180.0f / PI);
  Vector2 mvmt = {std::cos(angle), std::sin(angle)};
  auto prj = GetProjectileInstance(type, targetPos, true, GetSkillDamage(type), &PLAYER,
                                   mvmt, pov, {});
  SetDamageStats(prj, PLAYER_STATS.effects[CRIT_CHANCE],
                 PLAYER_STATS.effects[CRIT_DAMAGE_P]);
  PROJECTILES.emplace_back(prj);
  ApplyTalentsToCast(this);
}
void Skill::SkillAtPlayer(ProjectileType type, bool isFree) noexcept {
  if (!HandleCasting(isFree)) return;
  UseResources(isFree);
  Point targetPos = {
      PLAYER_X - typeToInfo[skillStats.type].size.x / 2.0F + PLAYER.size.x / 2,
      PLAYER_Y - typeToInfo[skillStats.type].size.y / 2.0F + PLAYER.size.y / 2};
  float angle = std::atan2(
      MOUSE_POS.y - targetPos.x_ - DRAW_Y - typeToInfo[skillStats.type].size.x / 2,
      MOUSE_POS.x - targetPos.y_ - DRAW_X - typeToInfo[skillStats.type].size.y / 2);
  float pov = angle * (180.0f / PI);
  Vector2 mvmt = {std::cos(angle), std::sin(angle)};
  auto prj = GetProjectileInstance(type, targetPos, true, GetSkillDamage(type), &PLAYER,
                                   mvmt, pov, {});
  SetDamageStats(prj, PLAYER_STATS.effects[CRIT_CHANCE],
                 PLAYER_STATS.effects[CRIT_DAMAGE_P]);
  PROJECTILES.emplace_back(prj);
}



#endif  //MAGE_QUEST_SRC_ENTITIES_PLAYER_H_
