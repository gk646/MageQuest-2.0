#ifndef MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_STATLOADER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_STATLOADER_H_
namespace StatLoader {
static void Load() noexcept {
  PLAYER_SECOND_STATS.LoadExperience(
      DataBaseHandler::GetNumFromTable("PLAYER_STATS", (int)PlayerSave::EXPERIENCE));
  PLAYER_SECOND_STATS.coins =
      DataBaseHandler::GetNumFromTable("PLAYER_STATS", (int)PlayerSave::COINS);

  for (int i = 0; i < GameStatistic::GAME_STATISTICS_END; i++) {
    GAME_STATISTICS.statistics[i] =
        DataBaseHandler::GetNumFromTable("GAME_STATISTICS", i + 1);
  }
}
}  // namespace StatLoader
#endif  //MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_STATLOADER_H_
