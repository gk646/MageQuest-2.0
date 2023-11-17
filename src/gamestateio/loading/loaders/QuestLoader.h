#ifndef MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_QUESTLOADER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_QUESTLOADER_H_
namespace QuestLoader {
static void load() noexcept {
  Quests::TUTORIAL = ScriptParser::load("Quests/FirstSteps.mgqs", Quest_ID::TUTORIAL);
  Quests::MARLA_LOST_NECKLACE =
      ScriptParser::load("Quests/Marla.mgqs", Quest_ID::MARLA, true);
  Quests::START_SOMETHING_NEW =
      ScriptParser::load("Quests/StartSomethingNew.mgqs", Quest_ID::START_SOMETHING_NEW);

}
}  // namespace QuestLoader
#endif  //MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_QUESTLOADER_H_
