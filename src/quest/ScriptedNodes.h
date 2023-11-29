#ifndef MAGEQUEST_SRC_QUEST_SCRIPTEDNODES_H_
#define MAGEQUEST_SRC_QUEST_SCRIPTEDNODES_H_

//Persistent info to allow for scripting inside function
struct ScriptMemory {
  void* ptrs[5] = {nullptr};
  int ints[5] = {0};
  bool bools[5] = {false};
};

//Function typedef
typedef bool (*FunctionPointer)(ScriptMemory&);

bool StoneKnightFightWithAria(ScriptMemory& mem) {
  //Get ptrs to the entities
  if (!mem.bools[0]) {
    for (auto& m : MONSTERS) {
      if (m->type == MonsterType::BOSS_STONE_KNIGHT) {
        mem.ptrs[0] = m;
        break;
      }
    }
    for (auto& n : NPCS) {
      if (n->id == NPC_ID::ARIA) {
        mem.ptrs[1] = n;
        break;
      }
    }
    if (!(mem.ptrs[0] && mem.ptrs[1])) return true;  //Exit if we cant find them
    mem.ptrs[2] = new std::string(
        "Oh no, hes looking at me! Where's my necklace... There! ... ... What?! ... "
        "*ughh*...");
    mem.bools[0] = true;
  }
  auto boss = ((Monster*)mem.ptrs[0]);
  auto aria = ((NPC*)mem.ptrs[1]);

  //Routines executed based on bools
  if (mem.bools[1]) {
    mem.ints[0]++;
    if (mem.ints[0] > 2000) return true;  //Skip cutscene on malfunction
    PLAYER_EFFECTS.AddEffect(new Stun(1), true);
    boss->threatManager.Reset();
    boss->threatManager.AddTarget(aria, 5.0F);
    boss->attackComponent.currentCooldown = 2;
    if (mem.ints[1] > 140 && mem.ints[1] < 170) {

      auto& val = boss->attackComponent.attacks[4]->currentCooldown;
      val = std::max((int16_t)50, val);
    }
    boss->attackComponent.currentCooldown++;
    //make aria stop running
    if (boss->tilePos.dist(aria->tilePos) < 1) {
      mem.ints[1]++;
      if (mem.ints[1] == 30) {
        aria->UpdateDialogue((std::string*)mem.ptrs[2]);
      }
      if (mem.ints[1] == 190) {
        aria->spriteCounter = 0;
        aria->actionState = -100;
      }

      if (mem.ints[1] == 290) {
        aria->dialogue = nullptr;
        aria->dialogueProgressCount = 1000.0F;
        delete (std::string*)mem.ptrs[2];
        boss->threatManager.Reset();
        return true;
      }
    }
  }

  if (boss->stats.health <= 0.95 * boss->stats.GetMaxHealth()) {
    mem.bools[1] = true;
  }
  return false;
}

std::array<FunctionPointer, 1> functionArray{StoneKnightFightWithAria};

#endif  //MAGEQUEST_SRC_QUEST_SCRIPTEDNODES_H_
