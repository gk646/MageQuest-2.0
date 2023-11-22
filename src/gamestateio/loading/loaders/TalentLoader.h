#ifndef MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_TALENTLOADER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_TALENTLOADER_H_

namespace TalentLoader {
//Loads the activated talents from the table
static void LoadActivatedTalents(std::vector<TalentNode>& talents) noexcept {
  sqlite3_stmt* stmt;
  std::string sql = "SELECT * FROM PLAYER_TALENTS";
  if (!DataBaseHandler::PrepareStmt(sql, DataBaseHandler::gameSave, &stmt)) return;
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    int index = sqlite3_column_int(stmt, 0);
    auto& node = talents[index];
    node.isActivated = true;
    PLAYER_STATS.EquipItem(node.talent.effects);
    if (node.talent.talentEffect) {
      UNIQUE_EFFECTS.push_back(node.talent.talentEffect->OnAdd());
    }
  }
  sqlite3_finalize(stmt);
}
//Loads all connections as specified in the file
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
  auto& talents = TALENTS;
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    TalentSize size = TalentSize::NORMAL;
    auto x = (float)sqlite3_column_int(stmt, 4);
    auto y = (float)sqlite3_column_int(stmt, 5);
    auto id = (int16_t)sqlite3_column_int(stmt, 0);
    if (id > 180) {
      x -= 1350;
      y -= 1350;
    }
    if (sqlite3_column_int(stmt, 7)) {
      size = TalentSize(sqlite3_column_int(stmt, 7));
    }
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
      for (const auto num : CRITICAL_STATS) {
        if (node.talent.effects[num] != 0.0F) {
          node.talent.hasModifiableStats = true;
          break;
        }
      }
    }
    if (talentIDToEffect.contains(node.talentID)) {
      node.talent.talentEffect = talentIDToEffect[node.talentID];
    }
    talents.emplace_back(node);
  }
  sqlite3_finalize(stmt);
  LoadActivatedTalents(talents);
}
}  // namespace TalentLoader
#endif  //MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_TALENTLOADER_H_
