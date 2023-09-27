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
    auto id = NPC_ID(sqlite3_column_int(stmt, 0));
    auto x = static_cast<float>(sqlite3_column_double(stmt, 1));
    auto y = static_cast<float>(sqlite3_column_double(stmt, 2));

    switch (id) {
      case NPC_ID::DECKARD:
        NPCS.push_back(new Deckard(x, y));
        break;
      case NPC_ID::MARLA:
        break;
      case NPC_ID::ARIA:
        break;
      case NPC_ID::HILLCREST_MAYOR:
        break;
      case NPC_ID::RECEPTIONS:
        break;
      case NPC_ID::DYING_MAN:
        break;
      case NPC_ID::NIETZSCHE:
        break;
      case NPC_ID::VILLAGER:
        break;
      case NPC_ID::TRADER:
        break;
      case NPC_ID::RANDOM:
        break;
      case NPC_ID::END:
        break;
    }
  }
  sqlite3_finalize(stmt);
}

}  // namespace NPCLoader
#endif  //MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_ENTITYSTATELOADER_H_
