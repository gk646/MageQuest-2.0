#ifndef MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_TALENTLOADER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_TALENTLOADER_H_

namespace TalentLoader {
static void LoadConnections() noexcept {
  auto table = Util::ReadMGI("mgi/TalentConnections.mgi");
  auto& matrix = UI_MANAGER.playerUI.talentPanel.matrix;
  for (const auto& vec : table) {
    matrix.MakeEdge((int16_t)std::stoi(vec[1]), (int16_t)std::stoi(vec[0]));
  }
}
//Loads all the talents from the TALENT table and creates them inside the "talentPanel"
static void Load() noexcept {
  sqlite3_stmt* stmt;
  std::string sql = "SELECT * FROM TALENTS ORDER BY id";
  if (!DataBaseHandler::PrepareStmt(sql, DataBaseHandler::dataBase, &stmt)) return;
  auto& talents = UI_MANAGER.playerUI.talentPanel.talents;
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    TalentSize size = TalentSize::NORMAL;
    auto x = (float)sqlite3_column_int(stmt, 4);
    auto y = (float)sqlite3_column_int(stmt, 5);
    auto id = (int16_t)sqlite3_column_int(stmt, 0);
    if (sqlite3_column_int(stmt, 7)) {
      size = TalentSize(sqlite3_column_int(stmt, 7));
    }
    if (id > 178) break;
    TalentNode node{{x, y}, size, id};
    node.talent.description = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
    node.talent.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    const char* iconPath = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    snprintf(TEXT_BUFFER, TEXT_BUFFER_SIZE, "%s%s%s", ASSET_PATH.c_str(),
             "ui/talents/TalentIcons/", iconPath);
    node.talent.icon = LoadTexture(TEXT_BUFFER);
    if (sqlite3_column_text(stmt, 6) != nullptr) {
      std::string effect = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
      Item::ParseAttributeStats(node.talent.effects, effect);
      Item::ParseEffectText(node.talent.effects, sqlite3_column_text(stmt, 6));
    }

    talents.emplace_back(node);
  }
  sqlite3_finalize(stmt);
}
}  // namespace TalentLoader
#endif  //MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_TALENTLOADER_H_