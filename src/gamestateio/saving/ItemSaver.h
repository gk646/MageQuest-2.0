#ifndef MAGEQUEST_SRC_GAMESTATEIO_SAVING_ITEMSAVER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_SAVING_ITEMSAVER_H_
struct ItemSaver {

  static void save() noexcept {
    save_items(PLAYER_EQUIPPED, 10, "PLAYER_INV");
    save_items(PLAYER_BAG, PLAYER_BAG_SIZE, "PLAYER_BAG");
  }
  static void save_items(InventorySlot* slots, int size,
                         const std::string& table) noexcept {
    sqlite3_stmt* stmt;
    std::string sql =
        "UPDATE " + table +
        " SET i_id = ?, type = ?, quality = ?, level = ?, effect = ? WHERE ROWID = ?";

    if (!DataBaseHandler::prepare_stmt(sql, DataBaseHandler::gamesave, &stmt))
      return;

    for (uint_fast32_t i = 0; i < size; i++) {
      if (slots[i].item) {
        sqlite3_bind_int(stmt, 1, slots[i].item->id);
        sqlite3_bind_int(stmt, 2, static_cast<int>(slots[i].item->type));
        sqlite3_bind_int(stmt, 3, slots[i].item->quality);
        sqlite3_bind_int(stmt, 4, slots[i].item->level);
        sqlite3_bind_text(stmt, 5, get_effect_text(slots[i].item->effects).c_str(), -1,
                          SQLITE_TRANSIENT);
      } else {
        for (int col = 1; col <= 5; ++col) {
          sqlite3_bind_null(stmt, col);
        }
      }
      sqlite3_bind_int(stmt, 6, i + 1);

      sqlite3_step(stmt);
      sqlite3_reset(stmt);
    }
    sqlite3_finalize(stmt);
  }
  inline static std::string get_effect_text(float* arr) {
    std::string effect_string;
    for (uint_fast32_t i = 0; i < STATS_ENDING; i++) {
      if (arr[i] != 0) {
        effect_string += std::to_string(i) + ":" + std::to_string(arr[i]) + ";";
      }
    }
    return effect_string;
  }

};
#endif  //MAGEQUEST_SRC_GAMESTATEIO_SAVING_ITEMSAVER_H_
