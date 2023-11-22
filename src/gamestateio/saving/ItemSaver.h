#ifndef MAGEQUEST_SRC_GAMESTATEIO_SAVING_ITEMSAVER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_SAVING_ITEMSAVER_H_
struct ItemSaver {
  static void Save() noexcept {
    SaveItemsToTable(PLAYER_EQUIPPED, 10, "PLAYER_INV");
    SaveItemsToTable(PLAYER_BAG, 60, "PLAYER_BAG");
    SaveItemsToTable(UI_MANAGER.playerUI.charBag.bagPanel.bagSlots.data(), 4,
                     "PLAYER_INV", 12);
  }
  static void SaveItemsToTable(ItemSlot* slots, int size, const std::string& table,
                               int offsetY = 0) noexcept {
    sqlite3_stmt* stmt;
    std::string sql =
        "UPDATE " + table +
        " SET i_id = ?, type = ?, quality = ?, level = ?, effect = ? WHERE ROWID = ?";

    if (!DataBaseHandler::PrepareStmt(sql, DataBaseHandler::gameSave, &stmt)) return;

    for (int i = 0; i < size; i++) {
      if (slots[i].item) {
        sqlite3_bind_int(stmt, 1, slots[i].item->id);
        sqlite3_bind_int(stmt, 2, static_cast<int>(slots[i].item->type));
        sqlite3_bind_int(stmt, 3, slots[i].item->quality);
        sqlite3_bind_int(stmt, 4, slots[i].item->level);
        sqlite3_bind_text(stmt, 5, GetEffectText(slots[i].item->effects).c_str(), -1,
                          SQLITE_TRANSIENT);
      } else {
        for (int col = 1; col <= 5; ++col) {
          sqlite3_bind_null(stmt, col);
        }
      }
      sqlite3_bind_int(stmt, 6, i + 1 + offsetY);

      sqlite3_step(stmt);
      sqlite3_reset(stmt);
    }
    sqlite3_finalize(stmt);
  }
  inline static std::string GetEffectText(const float* arr) noexcept {
    std::string effect_string;
    for (int i = 0; i < STATS_ENDING; i++) {
      if (arr[i] != 0) {
        effect_string += std::to_string(i) + ":" + std::to_string(arr[i]) + ";";
      }
    }
    return effect_string;
  }
};
#endif  //MAGEQUEST_SRC_GAMESTATEIO_SAVING_ITEMSAVER_H_
