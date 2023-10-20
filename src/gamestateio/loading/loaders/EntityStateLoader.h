#ifndef MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_ENTITYSTATELOADER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_ENTITYSTATELOADER_H_
namespace NPCLoader {

static void LoadNamedNPCs() {
  sqlite3_stmt* stmt;
  if (!DataBaseHandler::prepare_stmt("SELECT * FROM NPC_STATES",
                                     DataBaseHandler::gamesave, &stmt))
    return;

  while (sqlite3_step(stmt) == SQLITE_ROW) {
    if (sqlite3_column_type(stmt, 0) == SQLITE_NULL) break;
    auto npcID = NPC_ID(sqlite3_column_int(stmt, 0));
    auto absoluteX = static_cast<float>(sqlite3_column_double(stmt, 1));
    auto absoluteY = static_cast<float>(sqlite3_column_double(stmt, 2));
    auto zone = Zone(sqlite3_column_int(stmt, 3));
    NPCS.push_back(NPC::GetNPCInstance(npcID, absoluteX, absoluteY, zone));
  }
  sqlite3_finalize(stmt);
}

}  // namespace NPCLoader
#endif  //MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_ENTITYSTATELOADER_H_
