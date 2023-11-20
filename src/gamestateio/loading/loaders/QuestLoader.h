#ifndef MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_QUESTLOADER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_QUESTLOADER_H_
namespace QuestLoader {
static void LoadProgress() noexcept {
  sqlite3_stmt* stmt;
  DataBaseHandler::PrepareStmt("SELECT * FROM QUEST_FACTS", DataBaseHandler::gameSave,
                               &stmt);

  while (sqlite3_step(stmt) == SQLITE_ROW) {
    if (sqlite3_column_type(stmt, 2) != SQLITE_NULL) {
      auto quest = GetQuest(Quest_ID(sqlite3_column_int(stmt, 0)));
      quest->state = QuestState(sqlite3_column_int(stmt, 1));
      quest->stage = (int16_t)sqlite3_column_int(stmt, 2);
      if (sqlite3_column_type(stmt, 3) != SQLITE_NULL) {
        quest->pastDialogue = Quest::LoadQuestText(
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
      }
      PLAYER_QUESTS.AddQuest(quest);
    }
  }
  sqlite3_finalize(stmt);
}
static void Load() noexcept {
  Quests::TUTORIAL = ScriptParser::load("Quests/FirstSteps.mgqs", Quest_ID::TUTORIAL);
  Quests::MARLA_LOST_NECKLACE =
      ScriptParser::load("Quests/Marla.txt", Quest_ID::MARLA, true);
  Quests::START_SOMETHING_NEW =
      ScriptParser::load("Quests/StartSomethingNew.txt", Quest_ID::START_SOMETHING_NEW);

  LoadProgress();
  if (PLAYER_QUESTS.quests.empty()) {
    PLAYER_QUESTS.AddQuest(Quests::START_SOMETHING_NEW);
  } else {
    for (const auto& q : PLAYER_QUESTS.quests) {
      if (!q->hidden && q->state == QuestState::ACTIVE) {
        PLAYER_QUESTS.SetAsActiveQuest(q);
        UI_MANAGER.playerUI.questPanel.sidePanel.expanded = true;
      }
    }
  }
}
}  // namespace QuestLoader
#endif  //MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_QUESTLOADER_H_
