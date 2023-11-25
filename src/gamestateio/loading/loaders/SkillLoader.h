#ifndef MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_SKILLLOADER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_SKILLLOADER_H_
namespace SkillLoader {
inline static int ENUM_NAME_INDEX = 0;
inline static int DisplayName_INDEX = 1;
inline static int SpecificValue1_INDEX = 2;
inline static int SpecificValue2_INDEX = 3;
inline static int BaseDamage_INDEX = 4;
inline static int Cooldown_INDEX = 5;
inline static int Range_INDEX = 6;
inline static int ManaCost_INDEX = 7;
inline static int Description_INDEX = 8;

static void LoadLearnedSkills() noexcept {
  sqlite3_stmt* stmt;
  if (!DataBaseHandler::PrepareStmt("SELECT * FROM PLAYER_SKILLS",
                                    DataBaseHandler::gameSave, &stmt))
    return;

  while (sqlite3_step(stmt) == SQLITE_ROW) {
   SkillPanel::AddSkill(SKILLS[sqlite3_column_int(stmt, 0)]);
  }

  sqlite3_finalize(stmt);
}

static void Load() noexcept {
  auto ret = Util::ReadMGI("mgi/Skills.mgi");
  for (const auto& vec : ret) {
    auto type = stringToProjectile[vec[ENUM_NAME_INDEX]];
    SkillStats stats;
    stats.specialVal1 = std::stof(vec[SpecificValue1_INDEX]);
    stats.specialVal2 = std::stof(vec[SpecificValue2_INDEX]);
    stats.baseDamage = std::stof(vec[BaseDamage_INDEX]);
    stats.coolDownTicks = std::stoi(vec[Cooldown_INDEX]);
    stats.range = std::stoi(vec[Range_INDEX]);
    stats.manaCost = std::stof(vec[ManaCost_INDEX]);
    stats.type = type;
    stats.castTime = std::stoi(vec[9]);
    stats.dmgType = damageNamesToType[vec[10]];
    auto skillPtr = Skill::GetNewSkill(type, stats);
    if (skillPtr) {
      skillPtr->description = vec[Description_INDEX];
      skillPtr->name = vec[DisplayName_INDEX];
    }
    SKILLS[type] = skillPtr;
  }

  for (int i = 0; i < 6; i++) {
    PLAYER_HOTBAR[i] = new SkillSlot(48 + 65 * i, 20, SKILLS[LOCKED], false);
  }

  LoadLearnedSkills();
}
}  // namespace SkillLoader
#endif  //MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_SKILLLOADER_H_
