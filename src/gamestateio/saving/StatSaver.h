#ifndef MAGEQUEST_SRC_GAMESTATEIO_SAVING_STATSAVER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_SAVING_STATSAVER_H_

//Steps need to save player progress:

//Save Items -> ItemSaver
//Save QuestProgress -> QuestSaver
//Save XP, Coins, Statistics -> this
//Save Skills

namespace StatSaver {

static void SaveLearnedSkills() noexcept {
  std::string deleteSql = "DELETE FROM PLAYER_SKILLS";
  sqlite3_exec(DataBaseHandler::gameSave, deleteSql.c_str(), nullptr, nullptr, nullptr);

  sqlite3_stmt* stmt;
  std::string insertSql = "INSERT INTO PLAYER_SKILLS (SKILL_NUM) VALUES (?)";
  if (!DataBaseHandler::PrepareStmt(insertSql, DataBaseHandler::gameSave, &stmt)) return;

  for (const auto& skill : PLAYER_SKILLS) {
    for (int i = 0; i < SKILLS.size(); i++) {
      if (skill->skill == SKILLS[i]) {
        sqlite3_bind_int(stmt, 1, i);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
          // Handle error here
        }
        sqlite3_reset(stmt);
      }
    }
  }

  sqlite3_finalize(stmt);
}
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

  SaveLearnedSkills();
}
}  // namespace StatSaver
#endif  //MAGEQUEST_SRC_GAMESTATEIO_SAVING_STATSAVER_H_
