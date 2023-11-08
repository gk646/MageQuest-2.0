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
    auto skillPtr = Skill::GetNewSkill(type, stats);
    skillPtr->description = vec[Description_INDEX];
    skillPtr->name = vec[DisplayName_INDEX];
    SKILLS[type] = skillPtr;
  }
}
}  // namespace SkillLoader
#endif  //MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_SKILLLOADER_H_
