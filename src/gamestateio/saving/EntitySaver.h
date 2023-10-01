#ifndef MAGEQUEST_SRC_GAMESTATEIO_SAVING_ENTITYSAVER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_SAVING_ENTITYSAVER_H_
struct EntitySaver {};

namespace NPCSaver {
inline static constexpr int NAMED_LIMIT = 20;
inline static void AddNULLRow(sqlite3_stmt* stmt, int columns, int row) noexcept {
  for (uint_fast32_t i = 0; i < row; i++) {
    sqlite3_bind_null(stmt, i);
  }
  sqlite3_bind_int(stmt, columns, row);
}
inline static void SaveNPC(const NPC* npc, sqlite3_stmt* stmt, int i) noexcept {
  sqlite3_bind_int(stmt, 1, (int)npc->id);
  sqlite3_bind_int(stmt, 2, (int)npc->pos.x_);
  sqlite3_bind_int(stmt, 3, (int)npc->pos.y_);
  sqlite3_bind_int(stmt, 4, (int)npc->zone);
  sqlite3_bind_int(stmt, 5, i);

  sqlite3_step(stmt);
  sqlite3_reset(stmt);
}
static void SaveNamedNPCs() noexcept {
  sqlite3_stmt* stmt;
  std::string sql =
      "UPDATE NPC_STATES SET TYPE = ?, X_POS = ?, Y_POS = ?, ZONE = ? WHERE ROWID = ?";

  if (!DataBaseHandler::prepare_stmt(sql, DataBaseHandler::gamesave, &stmt)) return;
  int i = 1;
  for (auto& npc : NPCS) {
    if (npc->id != NPC_ID::RANDOM && npc->id != NPC_ID::VILLAGER &&
        npc->id != NPC_ID::TRADER) {
      SaveNPC(npc, stmt, i);
      i++;
    }
  }
  for (int j = i; j < NAMED_LIMIT; j++) {
    AddNULLRow(stmt, 4,j);
  }
  sqlite3_finalize(stmt);
}
}  // namespace NPCSaver
#endif  //MAGEQUEST_SRC_GAMESTATEIO_SAVING_ENTITYSAVER_H_
