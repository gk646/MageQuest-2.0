#ifndef MAGEQUEST_SRC_GAMESTATEIO_SAVING_ENTITYSAVER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_SAVING_ENTITYSAVER_H_
namespace NPCSaver {
inline static constexpr int NAMED_LIMIT = 30;
inline static void SaveNPC(const NPC* npc, sqlite3_stmt* stmt) noexcept {
  sqlite3_bind_int(stmt, 1, (int)npc->id);
  sqlite3_bind_int(stmt, 2, (int)npc->pos.x_);
  sqlite3_bind_int(stmt, 3, (int)npc->pos.y_);
  sqlite3_bind_int(stmt, 4, (int)npc->currentZone);

  sqlite3_step(stmt);
  sqlite3_reset(stmt);
}
static void SaveNamedNPCs() noexcept {
  sqlite3_stmt* stmt;

  std::string sqlDelete = "DELETE FROM NPC_STATES";
  if (!DataBaseHandler::PrepareStmt(sqlDelete, DataBaseHandler::gameSave, &stmt)) return;
  if (sqlite3_step(stmt) != SQLITE_DONE) {
    std::cerr << "Error deleting rows." << std::endl;
  }
  sqlite3_finalize(stmt);

  std::string sqlInsert =
      "INSERT INTO NPC_STATES (TYPE, X_POS, Y_POS, ZONE) VALUES (?, ?, ?, ?)";
  if (!DataBaseHandler::PrepareStmt(sqlInsert, DataBaseHandler::gameSave, &stmt)) return;

  for (auto& npc : NPCS) {
    if (npc->id != NPC_ID::RANDOM && npc->id != NPC_ID::VILLAGER &&
        npc->id != NPC_ID::TRADER) {
      SaveNPC(npc,  stmt);
    }
  }
  sqlite3_finalize(stmt);
}
}  // namespace NPCSaver
#endif  //MAGEQUEST_SRC_GAMESTATEIO_SAVING_ENTITYSAVER_H_
