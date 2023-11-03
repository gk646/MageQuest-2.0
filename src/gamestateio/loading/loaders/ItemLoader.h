#ifndef MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_ITEMLOADER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_ITEMLOADER_H_
namespace ItemLoader {
//Creates new base items from the database tables
static void CreateItemsFromTable(const std::string& table) noexcept {
  sqlite3_stmt* stmt;
  if (!DataBaseHandler::PrepareStmt("SELECT * FROM " + table, DataBaseHandler::dataBase,
                                    &stmt))
    return;

  while (sqlite3_step(stmt) == SQLITE_ROW) {
    const char* name_ptr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    if (name_ptr) {
      const char* description_ptr =
          reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
      const char* texturePath_ptr =
          reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));

      std::string texturePath = ASSET_PATH + texturePath_ptr + ".png";

      auto& s = ITEMS.emplace_back(sqlite3_column_int(stmt, 0), name_ptr,
                                   ItemRarity(sqlite3_column_int(stmt, 2)),
                                   ItemType(sqlite3_column_int(stmt, 3)), description_ptr,
                                   LoadTexture(texturePath.c_str()));
      Item::ParseEffectText(s.effects, sqlite3_column_text(stmt, 7));
      Item::ParseAttributeStats(
          s.effects, reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6)));
    }
  }
  sqlite3_finalize(stmt);
}
//Loads saved items from the bag or inventory
static void LoadItemsFromTable(InventorySlot* slots, const std::string& table, int length,
                               bool equip = false, int offsetY = 0) noexcept {
  sqlite3_stmt* stmt;
  if (!DataBaseHandler::PrepareStmt("SELECT * FROM " + table, DataBaseHandler::gameSave,
                                    &stmt))
    return;

  for (int i = 0; i < length + offsetY && sqlite3_step(stmt) == SQLITE_ROW; ++i) {
    if (i < offsetY) continue;

    slots[i - offsetY].item = Item::FindBaseItem(
        sqlite3_column_int(stmt, 0), ItemType(sqlite3_column_int(stmt, 1)),
        sqlite3_column_int(stmt, 2), sqlite3_column_int(stmt, 3));

    if (slots[i - offsetY].item) {
      //Clear any previous effects
      std::fill(slots[i - offsetY].item->effects,
                slots[i - offsetY].item->effects + STATS_ENDING, 0.0f);
      Item::ParseEffectText(slots[i - offsetY].item->effects,
                            sqlite3_column_text(stmt, 4));
      if (equip) {
        PLAYER_STATS.EquipItem(slots[i - offsetY].item->effects);
      }
    }
  }
  sqlite3_finalize(stmt);
}
static void Load() noexcept {
  ITEMS.reserve(80);
  std::array<std::string, 11> table_names = {
      "ARM_AMULET",  "ARM_BAG",   "ARM_BOOTS",  "ARM_CHEST", "ARM_HEAD",    "ARM_OFFHAND",
      "ARM_ONEHAND", "ARM_PANTS", "ARM_RELICS", "ARM_RINGS", "ARM_TWOHANDS"};
  for (const auto& name : table_names) {
    CreateItemsFromTable(name);
  }
  RANGE_EXISTING_ITEMS = std::uniform_int_distribution<int>(0, (int)ITEMS.size() - 1);
  std::cout << "ITEMS LOADED: " << ITEMS.size() << std::endl;
  LoadItemsFromTable(PLAYER_EQUIPPED, "PLAYER_INV", 10, true);
  LoadItemsFromTable(UI_MANAGER.playerUI.charBag.bagPanel.bagSlots.data(), "PLAYER_INV",
                     4, true, 12);
  LoadItemsFromTable(PLAYER_BAG, "PLAYER_BAG", (int)PLAYER_STATS.GetBagSlots());
}
}  // namespace ItemLoader
#endif  //MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_ITEMLOADER_H_
