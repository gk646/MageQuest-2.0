#ifndef MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_QUESTLOADER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_QUESTLOADER_H_
namespace QuestLoader {
static void load() noexcept {

  QuestResources::TUTORIAL = ScriptParser::load("Quests/Tutorial.mgqs", Quest_ID::TUTORIAL);

}
}  // namespace QuestLoader
#endif  //MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_QUESTLOADER_H_
