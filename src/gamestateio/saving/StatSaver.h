#ifndef MAGEQUEST_SRC_GAMESTATEIO_SAVING_STATSAVER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_SAVING_STATSAVER_H_

//Steps need to save player progress:

//Save Items -> ItemSaver
//Save QuestProgress -> QuestSaver
//Save XP, Coins, Statistics -> this

namespace StatSaver {

static void Save() noexcept {
  //Needs to bei std::ceil so you cant downlevel when reloading
  DataBaseHandler::SaveNumToTable((int)std::ceil(PLAYER_SECOND_STATS.experience),
                                  "PLAYER_STATS", (int)PlayerSave::EXPERIENCE);
  DataBaseHandler::SaveNumToTable(PLAYER_SECOND_STATS.coins, "PLAYER_STATS",
                                  (int)PlayerSave::COINS);

  for (int i = 0; i < GameStatistic::GAME_STATISTICS_END; i++) {
    DataBaseHandler::SaveNumToTable((int)GAME_STATISTICS.statistics[i], "GAME_STATISTICS",
                                    i + 1);
  }
}
}  // namespace StatSaver
#endif  //MAGEQUEST_SRC_GAMESTATEIO_SAVING_STATSAVER_H_
