#ifndef MAGEQUEST_SRC_GAMESTATEIO_SAVING_ENTITYSAVER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_SAVING_ENTITYSAVER_H_
namespace NPCSaver {
inline static constexpr int NAMED_LIMIT = 30;
inline static void SaveNPC(const NPC* npc, sqlite3_stmt* stmt, int i) noexcept {
  sqlite3_bind_int(stmt, 1, (int)npc->id);
  sqlite3_bind_int(stmt, 2, (int)npc->pos.x_);
  sqlite3_bind_int(stmt, 3, (int)npc->pos.y_);
  sqlite3_bind_int(stmt, 4, (int)npc->currentZone);
  sqlite3_bind_int(stmt, 5, i);

  sqlite3_step(stmt);
  sqlite3_reset(stmt);
}
static void SaveNamedNPCs() noexcept {
  sqlite3_stmt* stmt;
  std::string sql =
      "UPDATE NPC_STATES SET TYPE = ?, X_POS = ?, Y_POS = ?, ZONE = ? WHERE ROWID = ?";

  if (!DataBaseHandler::PrepareStmt(sql, DataBaseHandler::gameSave, &stmt)) return;
  int i = 1;
  for (auto& npc : NPCS) {
    if (npc->id != NPC_ID::RANDOM && npc->id != NPC_ID::VILLAGER &&
        npc->id != NPC_ID::TRADER) {
      SaveNPC(npc, stmt, i);
      i++;
    }
  }
  for (int j = i; j < NAMED_LIMIT; j++) {
    DataBaseHandler::AddNULLRow(stmt, 4, j);
  }
  sqlite3_finalize(stmt);
}
}  // namespace NPCSaver
#endif  //MAGEQUEST_SRC_GAMESTATEIO_SAVING_ENTITYSAVER_H_
