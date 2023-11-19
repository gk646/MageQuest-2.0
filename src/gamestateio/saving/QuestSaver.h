#ifndef MAGEQUEST_SRC_GAMESTATEIO_SAVING_QUESTSAVER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_SAVING_QUESTSAVER_H_
namespace QuestSaver {
static void Save() noexcept {
  sqlite3_stmt* stmt;
  DataBaseHandler::PrepareStmt(
      "UPDATE QUEST_FACTS SET State = ?, Stage = ?, JournalText = ? WHERE QUEST_ID = ?",
      DataBaseHandler::gameSave, &stmt);

  for (const auto& quest : PLAYER_QUESTS.quests) {
    sqlite3_bind_int(stmt, 1, (int)quest->state);
    sqlite3_bind_int(stmt, 2, (int)quest->stage);
    sqlite3_bind_text(stmt, 3, QuestText::SaveQuestText(quest->pastDialogue).c_str(), -1,
                      SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 4, (int)quest->id);
    if (sqlite3_step(stmt) != SQLITE_DONE) {}

    sqlite3_reset(stmt);
  }

  sqlite3_finalize(stmt);
}
}  // namespace QuestSaver
#endif  //MAGEQUEST_SRC_GAMESTATEIO_SAVING_QUESTSAVER_H_
