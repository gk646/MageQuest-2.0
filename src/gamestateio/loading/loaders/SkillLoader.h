#ifndef MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_SKILLLOADER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_SKILLLOADER_H_
namespace SkillLoader {
inline static int ENUM_NAME_INDEX = 0;
inline static int DisplayName_INDEX = 1;
inline static int SpecificValue1_INDEX = 2;
inline static int SpecificValue2_INDEX = 3;
inline static int BaseDamage_INDEX = 4;
inline static int LifeSpan_INDEX = 5;
inline static int Speed_INDEX = 6;
inline static int Cooldown_INDEX = 7;
inline static int Range_INDEX = 8;
inline static int ManaCost_INDEX = 9;
inline static int Description_INDEX = 10;

static void Load() noexcept {
  auto ret = Util::ReadMGI("mgi/Skills.mgi");
  for (const auto& vec : ret) {
    auto type = stringToProjectile[vec[0]];
    SkillStats stats;
    stats.specialVal1 = std::stof(vec[SpecificValue1_INDEX]);
    stats.specialVal2 = std::stof(vec[SpecificValue2_INDEX]);
    stats.baseDamage = std::stof(vec[BaseDamage_INDEX]);
    stats.lifeSpan = std::stoi(vec[LifeSpan_INDEX]);
    stats.speed = std::stof(vec[Speed_INDEX]);
    stats.coolDownTicks = std::stoi(vec[Cooldown_INDEX]);
    stats.range = std::stoi(vec[Range_INDEX]);
    stats.manaCost = std::stof(vec[ManaCost_INDEX]);
    auto skillPtr = Skill::GetNewSkill(type, stats);
    skillPtr->description = vec[Description_INDEX];
    skillPtr->name = vec[DisplayName_INDEX];
    SKILLS[type] = skillPtr;
  }
}
}  // namespace SkillLoader
#endif  //MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_SKILLLOADER_H_
